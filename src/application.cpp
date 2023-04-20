#include "application.hpp"
#include "vk/device.hpp"
#include "vk/instance.hpp"
#include <vulkan/vulkan_core.h>

void Application::Run() {
  InitVulkan();

  Prepare();
}

void Application::InitVulkan() { CreateInstance(); }

void Application::CreateInstance() {
  vk::InstanceCreateInfo create_info;

  create_info.layers.push_back("VK_LAYER_KHRONOS_validation");
  create_info.extensions.push_back("VK_EXT_debug_utils");

  uint32_t glfw_extensions_count;
  const char **glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

  for (int i = 0; i < glfw_extensions_count; i++) {
    create_info.extensions.push_back(glfw_extensions[i]);
  }

  instance = unique_ptr<vk::Instance>(new vk::Instance(create_info));
}

void Application::CreateDevice() {
  vk::PhysicalDevice *physical_device = instance->physical_devices[0].get();

  vk::DeviceCreateInfo create_info;
  create_info.extensions.clear();
  create_info.layers.clear();
  create_info.features = {0};
  create_info.queue_family = 111;
  
}

void Application::Prepare() {}
