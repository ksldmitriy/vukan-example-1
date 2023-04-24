#include "application.hpp"
#include "vk/device.hpp"
#include "vk/instance.hpp"
#include "vk/swapchain.hpp"
#include "window.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

void Application::Run() {
  window = unique_ptr<Window>(new Window());

  cout << "window created" << endl;
  
  InitVulkan();

  cout << "vulkan inited" << endl;

  window->CreateSurface(instance.get());

  cout << "surface created" << endl;

  swapchain = unique_ptr<vk::Swapchain>(
      new vk::Swapchain(device.get(), window->GetSurface()));

  Prepare();
}

void Application::InitVulkan() {
  CreateInstance();
  cout << "instace created " << endl;
  CreateDevice();
  cout << "device created" << endl;
}

void Application::Prepare() {}

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
