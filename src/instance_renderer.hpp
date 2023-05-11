#pragma once
#include "vk/vulkan.hpp"

class InstanceRenderer {
private:
  vk::Instance const *instance;

public:
  InstanceRenderer();
};
