#include "window.hpp"
#include "logs.hpp"
#include "vk/exception.hpp"
#include "vk/instance.hpp"

bool Window::glfw_inited = false;

Window::Window() {
  if (!glfw_inited) {
    InitGLFW();
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(900, 900, "title", 0, 0);
  if (!window) {
    cout << "cant create window" << endl;
    throw -1;
  }

  DEBUG("window created");
}

bool Window::ShouldClose() { return glfwWindowShouldClose(window); }

void Window::CreateSurface(vk::Instance *instance) {
  VkResult result =
      glfwCreateWindowSurface(instance->GetHandle(), window, nullptr, &surface);
  if (result) {
    throw CriticalException("cant create window surface");
  }

  DEBUG("surface created");
}

void Window::GetInstanceExtensions(const char **&extensions,
                                   uint32_t &extensions_count) {
  extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
}

VkSurfaceKHR Window::GetSurface() { return surface; }

void glfw_error_callback(int error, const char *description) {
  cout << "glfw error " << error << " :" << description << endl;
}

void Window::InitGLFW() {
  glfwSetErrorCallback(glfw_error_callback);

  int result = glfwInit();
  if (!result) {
    cout << "cant init glfw" << endl;
    throw -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}
