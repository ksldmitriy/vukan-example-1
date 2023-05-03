#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

using namespace std;

struct Vertex {
  glm::fvec2 pos;
  glm::fvec3 color;

  static VkVertexInputBindingDescription GetBindingDescription();
  static vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
};

struct InstanceData {
  glm::fvec2 pos;
  float rot;

  static VkVertexInputBindingDescription GetBindingDescription();
  static vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
};
