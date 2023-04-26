#pragma once
#include "device.hpp"
#include "exception.hpp"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

using namespace std;

namespace vk {

class Swapchain {
private:
  VkSwapchainKHR handle;
  VkSurfaceFormatKHR format;
  VkExtent2D extent;
  vector<VkImage> images;

  void GenerateCreateInfo(PhysicalDevice &physical_device,
                          VkSwapchainCreateInfoKHR &create_info,
                          VkSurfaceKHR surface, uint32_t desired_images_count,
                          VkSurfaceCapabilitiesKHR capabilities);
  void GetImages(Device &device);

  uint32_t ChooseSwapchainImagesCount(VkSurfaceCapabilitiesKHR &capabilities);
  void ChooseSurfaceFormat(vector<VkSurfaceFormatKHR> &supported_formats);
  VkPresentModeKHR
  ChoosePresentMode(vector<VkPresentModeKHR> &supported_present_modes);
  VkSurfaceTransformFlagBitsKHR
  ChooseSurfaceTransform(VkSurfaceCapabilitiesKHR surface_capabilities);
  VkExtent2D
  ChooseSwapchainExtend(VkSurfaceCapabilitiesKHR surface_capabilities);

public:
  Swapchain(Device &device, VkSurfaceKHR surface);
  Swapchain(Swapchain &) = delete;
  Swapchain &operator=(Swapchain &) = delete;

  VkSurfaceFormatKHR GetFormat();
};

} // namespace vk
