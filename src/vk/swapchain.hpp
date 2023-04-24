#pragma once
#include "exception.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

using namespace std;

namespace vk {

class Device;

class Swapchain {
private:
  VkSwapchainKHR handle;
  VkSurfaceFormatKHR format;
  VkExtent2D extent;
  vector<VkImage> images;

  VkSurfaceFormatKHR
  ChooseSurfaceFormat(vector<VkSurfaceFormatKHR> &supported_formats);
  VkPresentModeKHR
  ChoosePresentMode(vector<VkPresentModeKHR> &supported_present_modes);
  VkSurfaceTransformFlagBitsKHR
  ChooseSurfaceTransform(VkSurfaceCapabilitiesKHR surface_capabilities);
  VkExtent2D
  ChooseSwapchainExtend(VkSurfaceCapabilitiesKHR surface_capabilities);

public:
  Swapchain(Device *device, VkSurfaceKHR surface);
  Swapchain(Swapchain &) = delete;
  Swapchain &operator=(Swapchain &) = delete;
};

} // namespace vk
