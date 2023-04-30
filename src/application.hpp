#pragma once
#include "logs.hpp"
#include "vk/buffer.hpp"
#include "vk/command_buffer.hpp"
#include "vk/command_pool.hpp"
#include "vk/device.hpp"
#include "vk/device_memory.hpp"
#include "vk/exception.hpp"
#include "vk/instance.hpp"
#include "vk/shader_module.hpp"
#include "vk/swapchain.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

using namespace std;

struct Vertex {
  glm::fvec2 pos;
  glm::fvec3 color;

  static VkVertexInputBindingDescription GetBindingDescription() { // TODO
    VkVertexInputBindingDescription binding_description{};
    binding_description.binding = 0;
    binding_description.stride = sizeof(Vertex);
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return binding_description;
  }

  static array<VkVertexInputAttributeDescription, 2>
  GetAttributeDescriptions() {
    array<VkVertexInputAttributeDescription, 2> attribute_descriptions{};
    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[0].offset = offsetof(Vertex, pos);

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = offsetof(Vertex, color);

    return attribute_descriptions;
  }
};

class Application {
private:
  const int frames_in_flight = 2;

  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  VkSemaphore image_available_semaphore, render_finished_semaphore;
  VkFence fence;

  unique_ptr<Window> window;

  unique_ptr<vk::Swapchain> swapchain;

  unique_ptr<vk::DeviceMemory> vertex_buffer_memory;
  unique_ptr<vk::Buffer> vertex_buffer;

  vk::Queue graphics_queue;

  VkRenderPass render_pass;

  VkPipelineLayout pipeline_layout;

  VkPipeline pipeline;

  vector<VkFramebuffer> framebuffers;

  unique_ptr<vk::CommandPool> command_pool;
  vector<unique_ptr<vk::CommandBuffer>> command_buffers;

  void InitVulkan();
  void Prepare();
  void RenderLoop();
  void Draw();
  void Render(uint32_t next_image_index);
  void Present(uint32_t next_image_index);

  void CreateVertexBuffer();

  void CreateRenderPass();
  void InitFramebuffers();
  void CreateGraphicsPipeline();
  void CreateSyncObjects();
  void CreateCommandBuffer();

  void CreateInstance();
  void CreateDevice();

public:
  Application() = default;
  Application(Application &) = delete;
  Application &operator=(Application &) = delete;

  void Run();
};
