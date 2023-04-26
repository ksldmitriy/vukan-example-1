#pragma once
#include "vk/instance.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {
class Instance;
}

class Window {
private:
  static bool glfw_inited;

  GLFWwindow *window;
  VkSurfaceKHR surface;

  void InitGLFW();

public:
  Window();

  void CreateSurface(vk::Instance *instance);
  VkSurfaceKHR GetSurface();
  void GetInstanceExtensions(const char **&extensions,
                             uint32_t &extensions_count);

  bool ShouldClose();  
};
