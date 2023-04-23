#pragma once
#include "vk/exception.hpp"
#include "vk/instance.hpp"
#include "vk/device.hpp"
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

using namespace std;

class Application {
private:

  unique_ptr<vk::Instance> instance;
  unique_ptr<vk::Device> device;

  vk::Queue graphics_queue;
 
  void InitVulkan();
  void Prepare();

  void CreateInstance();
  void CreateDevice();
public:
  Application() = default;
  void Run();
};
