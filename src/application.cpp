#include "application.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

void Application::Run() {
  window = unique_ptr<Window>(new Window());

  InitVulkan();

  window->CreateSurface(instance.get());

  swapchain = unique_ptr<vk::Swapchain>(
      new vk::Swapchain(*device, window->GetSurface()));

  Prepare();
}

void Application::InitVulkan() {
  CreateInstance();

  CreateDevice();

  DEBUG("vulkan inited");
}

void Application::Prepare() { CreateRenderPass(); }

void Application::CreateRenderPass() {
  VkAttachmentDescription color_attachment;
  color_attachment.flags = 0;
  color_attachment.format = swapchain->GetFormat().format;
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
