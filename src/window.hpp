#pragma once
#include "vk/instance.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <vulkan/vulkan.h>

using namespace std;

namespace vk {
class Instance;
}

class Window {
private:
  static bool glfw_inited;
  static map<GLFWwindow *, Window *> windows_db;

  GLFWwindow *handle;
  VkSurfaceKHR surface;

  void InitGLFW();

  static void StaticKeyCallback(GLFWwindow *window, int key, int scancode,
                                int action, int mods);
  void KeyCallback(int key, int scancode, int action,
                   int mods);

public:
  Window();
  Window(Window &) = delete;
  Window &operator=(Window &) = delete;
  ~Window();

  void CreateSurface(vk::Instance *instance);
  VkSurfaceKHR GetSurface();
  void GetInstanceExtensions(const char **&extensions,
                             uint32_t &extensions_count);

  bool ShouldClose();
  void PollEvents();

  friend void glfw_key_callback(GLFWwindow *window, int key, int scancode,
                                int action, int mods);
};
