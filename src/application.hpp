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
#include <iostream>
#include <memory>
#include <chrono>
#include "render_structs.hpp"

chrono::high_resolution_clock::time_point typedef time_point;
chrono::high_resolution_clock::duration typedef duration;
const auto now = chrono::high_resolution_clock::now;

using namespace std;

class Application {
private:
  const int frames_in_flight = 2;

  time_point program_start;
  duration time_from_start;
  
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  VkSemaphore image_available_semaphore, render_finished_semaphore;
  VkFence fence;

  unique_ptr<Window> window;

  unique_ptr<vk::Swapchain> swapchain;

  unique_ptr<vk::DeviceMemory> vertex_buffer_memory, uniform_buffer_memory;
  unique_ptr<vk::Buffer> vertex_buffer, instance_buffer, uniform_buffer;

  vk::Queue graphics_queue;
  VkRenderPass render_pass;

  VkDescriptorSetLayout descriptor_set_layout;
  VkDescriptorPool descriptors_pool;
  VkDescriptorSet descriptor_set;
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

  void PreUpdate();
  void Update();
  void UpdateRenderData();

  void CreateUniformBuffer();
  void CreateDescriptorSetLayout();
  void CreateDescriptorPool();
  void AllocateDescriptorSet();
  void CreateDescriptors();
  void UpdateDescriptorSet();
  
  void CreateVertexInputBuffers();
  
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
