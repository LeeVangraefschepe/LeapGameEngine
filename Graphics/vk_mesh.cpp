#include "vk_mesh.h"

using namespace leap::graphics;

VertexInputDescription Vertex::GetVertexDescription()
{
    VertexInputDescription desc;

    // We only have one vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription mainBinding{};
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    desc.bindings.emplace_back(mainBinding);

    // Position will be stored at Location 0
    VkVertexInputAttributeDescription positionAttribute{};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    // Normal will be stored at Location 1
    VkVertexInputAttributeDescription normalAttribute{};
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, normal);

    // Color will be stored at Location 2
    VkVertexInputAttributeDescription colorAttribute{};
    colorAttribute.binding = 0;
    colorAttribute.location = 2;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    desc.attributes.emplace_back(positionAttribute);
    desc.attributes.emplace_back(normalAttribute);
    desc.attributes.emplace_back(colorAttribute);

    return desc;
}

bool Mesh::LoadFromObj(const char* filename)
{
    return false;
}
