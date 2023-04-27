#pragma once
#include "vk/device.hpp"
#include "vk/exception.hpp"
#include "vk/instance.hpp"
#include "vk/swapchain.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "logs.hpp"
#include "vk/command_pool.hpp"
#include "vk/shader_module.hpp"
#include "vk/command_pool.hpp"
#include "vk/command_buffer.hpp"

using namespace std;

class Application {
private:
  const int frames_in_flight = 2;

  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  VkSemaphore image_available_semaphore, render_finished_semaphore;
  VkFence fence;

  unique_ptr<Window> window;

  unique_ptr<vk::Swapchain> swapchain;

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
