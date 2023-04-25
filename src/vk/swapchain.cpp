#include "swapchain.hpp"
#include "device.hpp"
#include "templates.hpp"
#include "../logs.hpp"

namespace vk {

Swapchain::Swapchain(Device *device, VkSurfaceKHR surface) {
  VkResult result;

  vk::PhysicalDevice &physical_device = device->GetPhysicalDevice();

  // get capabilities
  VkSurfaceCapabilitiesKHR surface_capabilities =
      physical_device.GetSurfaceCapabilities(surface);
  vector<VkSurfaceFormatKHR> supported_formats =
      physical_device.GetSurfaceFormats(surface);
  vector<VkPresentModeKHR> supported_present_modes =
      physical_device.GetSurfacePresentModes(surface);

  uint32_t desired_swapchain_images_count =
      surface_capabilities.minImageCount + 1;
  if (surface_capabilities.maxImageCount &&
      desired_swapchain_images_count > surface_capabilities.maxImageCount) {
    desired_swapchain_images_count = surface_capabilities.maxImageCount;
  }

  // choose swapchain details
  format = ChooseSurfaceFormat(supported_formats);

  VkPresentModeKHR present_mode = ChoosePresentMode(supported_present_modes);

  VkSurfaceTransformFlagBitsKHR surface_transform =
      ChooseSurfaceTransform(surface_capabilities);

  extent = ChooseSwapchainExtend(surface_capabilities);

  VkSwapchainCreateInfoKHR create_info = vk::swapchain_create_info_template;
  create_info.surface = surface;
  create_info.minImageCount = desired_swapchain_images_count;
  create_info.imageFormat = format.format;
  create_info.imageColorSpace = format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  create_info.preTransform = surface_transform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = nullptr;

  result =
      vkCreateSwapchainKHR(device->GetHandle(), &create_info, nullptr, &handle);
  if (result) {
    throw CriticalException("cant create swapchain");
  }

  DEBUG("swapchain created");

  uint32_t swapchain_images_count = 0;
  result = vkGetSwapchainImagesKHR(device->GetHandle(), handle,
                                   &swapchain_images_count, nullptr);
  if (result) {
    throw CriticalException("cant get swapchain images count");
  } else if (!swapchain_images_count) {
    throw CriticalException("swapchain images count is 0");
  }

  TRACE("swapchain images count is {}", swapchain_images_count);

  images.resize(swapchain_images_count);

  result = vkGetSwapchainImagesKHR(device->GetHandle(), handle,
                                   &swapchain_images_count, images.data());
  if (result) {
    throw CriticalException("cant get swapchain images");
  }
}

VkExtent2D Swapchain::ChooseSwapchainExtend(
    VkSurfaceCapabilitiesKHR surface_capabilities) {
  return surface_capabilities.currentExtent;
}

VkSurfaceTransformFlagBitsKHR Swapchain::ChooseSurfaceTransform(
    VkSurfaceCapabilitiesKHR surface_capabilities) {
  return surface_capabilities.currentTransform;
}

VkPresentModeKHR Swapchain::ChoosePresentMode(
    vector<VkPresentModeKHR> &supported_present_modes) {
  return supported_present_modes[0];
}

VkSurfaceFormatKHR
Swapchain::ChooseSurfaceFormat(vector<VkSurfaceFormatKHR> &supported_formats) {
  if (supported_formats.size() == 1 &&
      supported_formats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto &format : supported_formats) {
    if (format.format == VK_FORMAT_R8G8B8A8_UNORM) {
      return format;
    }
  }

  return supported_formats[0];
}

} // namespace vk
