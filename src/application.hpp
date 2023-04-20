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

  void InitVulkan();
  void Prepare();

  void CreateInstance();
public:
  Application() = default;
  void Run();
};
