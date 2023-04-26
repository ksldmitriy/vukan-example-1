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

using namespace std;

class Application {
private:
  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  unique_ptr<Window> window;

  unique_ptr<vk::Swapchain> swapchain;

  vk::Queue graphics_queue;

  VkRenderPass render_pass;

  vector<VkFramebuffer> framebuffers;
  
  void InitVulkan();
  void Prepare();

  void CreateRenderPass();
  void InitFramebuffers();
  void CreateGraphicsPipeline();
  
  void CreateInstance();
  void CreateDevice();

public:
  Application() = default;
  Application(Application &) = delete;
  Application &operator=(Application &) = delete;

  void Run();
};
