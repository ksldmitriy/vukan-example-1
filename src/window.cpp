#include "window.hpp"
#include "logs.hpp"
#include "vk/exception.hpp"
#include "vk/instance.hpp"

bool Window::glfw_inited = false;
map<GLFWwindow *, Window *> Window::windows_db;

Window::Window() {
  surface = VK_NULL_HANDLE;

  if (!glfw_inited) {
    InitGLFW();
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  handle = glfwCreateWindow(900, 900, "title", 0, 0);
  if (!handle) {
    throw CriticalException("cant create window");
  }

  glfwSetKeyCallback(handle, StaticKeyCallback);
  glfwSetWindowSizeCallback(handle, StaticResizeCallback);

  windows_db[handle] = this;

  DEBUG("window created");
}

Window::~Window() {
  if (!handle) {
    return;
  }

  Destroy();
}

void Window::AttachInstance(vk::Instance &instance) {
  this->instance = &instance;
}

void Window::Destroy() {
  windows_db.erase(handle);

  glfwDestroyWindow(handle);

  if(surface){
	DestroySurface();
  }
  
  DEBUG("window destroyed");
}

void Window::DestroySurface(){
  vkDestroySurfaceKHR(instance->GetHandle(), surface, nullptr);
  surface = nullptr;
}

void Window::StaticResizeCallback(GLFWwindow *window, int width, int height) {
  if (!windows_db.contains(window)) {
    INFO("no window for resize callback");
    return;
  }

  windows_db[window]->ResizeCallback(width, height);
}

void Window::ResizeCallback(int width, int height) {
  INFO("window resized to {} {}", width, height);
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
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
    return;
  }
}

void Window::PollEvents() { glfwPollEvents(); }

bool Window::ShouldClose() { return glfwWindowShouldClose(handle); }

void Window::CreateSurface() {
  if(surface){
	DestroySurface();
  }

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
