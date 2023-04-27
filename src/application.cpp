#include "application.hpp"

void Application::Run() {
  window = unique_ptr<Window>(new Window());

  InitVulkan();

  window->CreateSurface(instance.get());

  swapchain = unique_ptr<vk::Swapchain>(
      new vk::Swapchain(*device, window->GetSurface()));

  Prepare();

  RenderLoop();
}

void Application::InitVulkan() {
  CreateInstance();

  CreateDevice();

  DEBUG("vulkan inited");
}

void Application::Prepare() {
  CreateRenderPass();
  InitFramebuffers();
  CreateGraphicsPipeline();
  CreateSyncObjects();
  CreateCommandBuffer();
}

void Application::RenderLoop() {
  while (!window->ShouldClose()) {
    Draw();

	window->PollEvents();
  }
}

void Application::Draw() {
  uint32_t next_image_index =
      swapchain->AcquireNextImage(image_available_semaphore);

  Render(next_image_index);

  Present(next_image_index);

  vkWaitForFences(device->GetHandle(), 1, &fence, VK_TRUE, UINT64_MAX);
  vkResetFences(device->GetHandle(), 1, &fence);
}

void Application::Render(uint32_t next_image_index) {
  VkCommandBuffer command_buffer_handle =
      command_buffers[next_image_index]->GetHandle();

  VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

  VkSubmitInfo submit_info = vk::submit_info_template;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer_handle;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &image_available_semaphore;
  submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &render_finished_semaphore;

  VkResult result =
      vkQueueSubmit(graphics_queue.GetHandle(), 1, &submit_info, fence);
  if (result) {
    throw vk::CriticalException("cant submit to queue");
  }
}

void Application::Present(uint32_t next_image_index) {
  VkSwapchainKHR swapchain_handle = swapchain->GetHandle();

  VkPresentInfoKHR present_info = vk::present_info_template;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &render_finished_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapchain_handle;
  present_info.pImageIndices = &next_image_index;

  VkResult result =
      vkQueuePresentKHR(graphics_queue.GetHandle(), &present_info);
  if (result) {
    throw vk::CriticalException("cant present");
  }
}

void Application::CreateCommandBuffer() {
  command_pool = make_unique<vk::CommandPool>(*device, graphics_queue,
                                              framebuffers.size());

  command_buffers.resize(framebuffers.size());

  for (int i = 0; i < framebuffers.size(); i++) {
    unique_ptr<vk::CommandBuffer> command_buffer =
        command_pool->AllocateCommandBuffer(vk::CommandBufferLevel::primary);

    command_buffer->Begin();

    VkClearValue clear_value = {{{0, 0, 0, 1}}};

    VkRenderPassBeginInfo render_pass_begin_info =
        vk::render_pass_begin_info_template;
    render_pass_begin_info.renderPass = render_pass;
    render_pass_begin_info.framebuffer = framebuffers[i];
    render_pass_begin_info.renderArea.offset = {0, 0};
    render_pass_begin_info.renderArea.extent = swapchain->GetExtent();
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_value;

    vkCmdBeginRenderPass(command_buffer->GetHandle(), &render_pass_begin_info,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer->GetHandle(),
                      VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    vkCmdDraw(command_buffer->GetHandle(), 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer->GetHandle());

    command_buffer->End();

    command_buffers[i] = move(command_buffer);
  }

  TRACE("render command buffers created");
}

void Application::CreateSyncObjects() {
  VkSemaphoreCreateInfo semphore_create_info =
      vk::semaphore_create_info_template;

  VkFenceCreateInfo fence_create_info = vk::fence_create_info_template;

  VkResult result;

  result = vkCreateSemaphore(device->GetHandle(), &semphore_create_info,
                             nullptr, &image_available_semaphore);
  if (result) {
    throw vk::CriticalException("cant create image available semaphore");
  }

  result = vkCreateSemaphore(device->GetHandle(), &semphore_create_info,
                             nullptr, &render_finished_semaphore);
  if (result) {
    throw vk::CriticalException("cant create render fished semaphore");
  }

  result =
      vkCreateFence(device->GetHandle(), &fence_create_info, nullptr, &fence);
  if (result) {
    throw vk::CriticalException("cant create fence");
  }

  TRACE("image available semaphore created");
  TRACE("render finished semaphore created");
  TRACE("fence created");
}

void Application::CreateGraphicsPipeline() {
  unique_ptr<vk::ShaderModule> vertex_shader =
      make_unique<vk::ShaderModule>(*device, "shaders/vert.spv");
  unique_ptr<vk::ShaderModule> fragment_shader =
      make_unique<vk::ShaderModule>(*device, "shaders/frag.spv");

  VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info =
      vk::pipeline_shader_stage_create_info_template;
  vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertex_shader_stage_create_info.module = vertex_shader->GetHandle();
  vertex_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info =
      vk::pipeline_shader_stage_create_info_template;
  fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragment_shader_stage_create_info.module = fragment_shader->GetHandle();
  fragment_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[2] = {
      vertex_shader_stage_create_info, fragment_shader_stage_create_info};

  VkPipelineVertexInputStateCreateInfo vertex_input =
      vk::vertex_input_create_info_template;
  vertex_input.vertexBindingDescriptionCount = 0;
  vertex_input.pVertexBindingDescriptions = nullptr;
  vertex_input.vertexAttributeDescriptionCount = 0;
  vertex_input.pVertexAttributeDescriptions = 0;

  VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info =
      vk::pipeline_input_assembly_create_info_template;
  input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  VkViewport viewport;
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = swapchain->GetExtent().width;
  viewport.height = swapchain->GetExtent().height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;

  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = swapchain->GetExtent();

  VkPipelineViewportStateCreateInfo viewport_state_create_info =
      vk::pipeline_viewport_state_create_info_template;
  viewport_state_create_info.pViewports = &viewport;
  viewport_state_create_info.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info =
      vk::pipeline_rasterization_state_create_info_template;
  rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;

  VkPipelineMultisampleStateCreateInfo multisample_create_info =
      vk::pipeline_multisample_state_create_info_template;

  VkPipelineColorBlendAttachmentState color_blend_attachment;
  color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo color_blend_state_create_info =
      vk::pipeline_color_blend_state_create_info_template;
  color_blend_state_create_info.logicOpEnable = VK_FALSE;
  color_blend_state_create_info.attachmentCount = 1;
  color_blend_state_create_info.pAttachments = &color_blend_attachment;

  VkPipelineLayoutCreateInfo pipeline_layout_create_info =
      vk::pipeline_layout_create_info_template;
  pipeline_layout_create_info.setLayoutCount = 0;
  pipeline_layout_create_info.pSetLayouts = nullptr;
  pipeline_layout_create_info.pushConstantRangeCount = 0;
  pipeline_layout_create_info.pPushConstantRanges = nullptr;

  VkResult result =
      vkCreatePipelineLayout(device->GetHandle(), &pipeline_layout_create_info,
                             nullptr, &pipeline_layout);
  if (result) {
    throw vk::CriticalException("cant create pipeline layout");
  }

  TRACE("pipeline layout created");

  VkPipelineDynamicStateCreateInfo dynamic_state =
      vk::pipeline_dynamic_state_create_info_template;
  dynamic_state.dynamicStateCount = 0;
  dynamic_state.pDynamicStates = nullptr;

  VkGraphicsPipelineCreateInfo pipeline_create_info =
      vk::graphics_pipeline_create_info_template;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = shader_stages;
  pipeline_create_info.pVertexInputState = &vertex_input;
  pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
  pipeline_create_info.pViewportState = &viewport_state_create_info;
  pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
  pipeline_create_info.pMultisampleState = &multisample_create_info;
  pipeline_create_info.pDepthStencilState = nullptr;
  pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
  pipeline_create_info.pDynamicState = &dynamic_state;
  pipeline_create_info.layout = pipeline_layout;
  pipeline_create_info.renderPass = render_pass;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_create_info.basePipelineIndex = -1;

  result = vkCreateGraphicsPipelines(device->GetHandle(), VK_NULL_HANDLE, 1,
                                     &pipeline_create_info, nullptr, &pipeline);
  if (result) {
    throw vk::CriticalException("cant create pipeline");
  }

  DEBUG("graphics pipeline created");
}

void Application::InitFramebuffers() {
  VkFramebufferCreateInfo create_info = vk::framebuffer_create_info_template;
  create_info.renderPass = render_pass;
  create_info.attachmentCount = 1;
  create_info.width = swapchain->GetExtent().width;
  create_info.height = swapchain->GetExtent().height;
  create_info.layers = 1;

  const vector<VkImageView> image_views = swapchain->GetImageViews();
  framebuffers.resize(image_views.size());

  for (int i = 0; i < framebuffers.size(); i++) {
    create_info.pAttachments = &image_views[i];
    VkResult result = vkCreateFramebuffer(device->GetHandle(), &create_info,
                                          nullptr, &framebuffers[i]);
    if (result) {
      throw vk::CriticalException("cant create framebuffer");
    }
  }

  DEBUG("framebuffers created");
}

void Application::CreateRenderPass() {
  VkAttachmentDescription color_attachment =
      vk::attachment_description_template;
  color_attachment.format = swapchain->GetFormat().format;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_reference;
  color_attachment_reference.attachment = 0;
  color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass_description = vk::subpass_description_template;
  subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass_description.colorAttachmentCount = 1;
  subpass_description.pColorAttachments = &color_attachment_reference;

  VkRenderPassCreateInfo create_info = vk::render_pass_create_info_template;
  create_info.attachmentCount = 1;
  create_info.pAttachments = &color_attachment;
  create_info.subpassCount = 1;
  create_info.pSubpasses = &subpass_description;

  VkResult result = vkCreateRenderPass(device->GetHandle(), &create_info,
                                       nullptr, &render_pass);
  if (result) {
    throw vk::CriticalException("cant create render pass");
  }

  DEBUG("render pass created");
}

void Application::CreateInstance() {
  vk::InstanceCreateInfo create_info;

  create_info.layers.push_back("VK_LAYER_KHRONOS_validation");
  create_info.extensions.push_back("VK_EXT_debug_utils");

  uint32_t glfw_extensions_count;
  const char **glfw_extensions;
  window->GetInstanceExtensions(glfw_extensions, glfw_extensions_count);

  for (int i = 0; i < glfw_extensions_count; i++) {
    create_info.extensions.push_back(glfw_extensions[i]);
  }

  instance = unique_ptr<vk::Instance>(new vk::Instance(create_info));
}

void Application::CreateDevice() {
  shared_ptr<vk::PhysicalDevice> physical_device =
      instance->physical_devices[0];

  vk::DeviceCreateInfo::QueueRequest graphics_queue_request;
  graphics_queue_request.flags = VK_QUEUE_GRAPHICS_BIT;
  graphics_queue_request.queue = &graphics_queue;

  vk::DeviceCreateInfo create_info;
  create_info.queue_requests.push_back(graphics_queue_request);

  device = unique_ptr<vk::Device>(new vk::Device(physical_device, create_info));
}
