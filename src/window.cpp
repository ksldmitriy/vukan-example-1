#include "window.hpp"
#include "logs.hpp"
#include "vk/exception.hpp"
#include "vk/instance.hpp"

bool Window::glfw_inited = false;
map<GLFWwindow *, Window *> Window::windows_db;

Window::Window() {
  if (!glfw_inited) {
    InitGLFW();
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  handle = glfwCreateWindow(900, 900, "title", 0, 0);
  if (!handle) {
    throw CriticalException("cant create window");
  }

  glfwSetKeyCallback(handle, StaticKeyCallback);

  windows_db[handle] = this;

  DEBUG("window created");
}

Window::~Window() {
  windows_db.erase(handle);

  glfwDestroyWindow(handle);

  DEBUG("window destroyed");
}

void Window::StaticKeyCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
  if (!windows_db.contains(window)) {
    INFO("no window for key callback");
    return;
  }

  windows_db[window]->KeyCallback(key, scancode, action, mods);
}

void Window::KeyCallback(int key, int scancode, int action, int mods) {
  if (key== GLFW_KEY_ESCAPE) {
	glfwSetWindowShouldClose(handle, GLFW_TRUE);
    return;
  }
}

void Window::PollEvents() { glfwPollEvents(); }

bool Window::ShouldClose() { return glfwWindowShouldClose(handle); }

void Window::CreateSurface(vk::Instance *instance) {
  VkResult result =
      glfwCreateWindowSurface(instance->GetHandle(), handle, nullptr, &surface);
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
