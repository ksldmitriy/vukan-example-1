#include "render_structs.hpp"

VkVertexInputBindingDescription Vertex::GetBindingDescription() { // TODO
  VkVertexInputBindingDescription binding_description{};
  binding_description.binding = 0;
  binding_description.stride = sizeof(Vertex);
  binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return binding_description;
}

vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions() {
  vector<VkVertexInputAttributeDescription> attribute_descriptions(2);
  attribute_descriptions[0].binding = 0;
  attribute_descriptions[0].location = 0;
  attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descriptions[0].offset = offsetof(Vertex, pos);

  attribute_descriptions[1].binding = 0;
  attribute_descriptions[1].location = 1;
  attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attribute_descriptions[1].offset = offsetof(Vertex, color);

  return attribute_descriptions;
}

VkVertexInputBindingDescription InstanceData::GetBindingDescription() { // TODO
  VkVertexInputBindingDescription binding_description{};
  binding_description.binding = 1;
  binding_description.stride = sizeof(InstanceData);
  binding_description.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

  return binding_description;
}

vector<VkVertexInputAttributeDescription>
InstanceData::GetAttributeDescriptions() {
  vector<VkVertexInputAttributeDescription> attribute_descriptions(2);
  attribute_descriptions[0].binding = 1;
  attribute_descriptions[0].location = 2;
  attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descriptions[0].offset = offsetof(InstanceData, pos);

  attribute_descriptions[1].binding = 1;
  attribute_descriptions[1].location = 3;
  attribute_descriptions[1].format = VK_FORMAT_R32_SFLOAT;
  attribute_descriptions[1].offset = offsetof(InstanceData, rot);

  return attribute_descriptions;
}
