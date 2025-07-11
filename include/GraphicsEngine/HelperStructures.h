#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <array>
#include <vector>
#include <optional>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, normal);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
    }
};


struct Transform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationAngle;
};

struct ModelCreateInfo {
    const char* modelPath;
};

struct Model {
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory* vertexBufferMemory;
    VkDeviceMemory* indexBufferMemory;
    uint32_t indicesCount;
    std::vector<Vertex> vertices;
};



struct PipelineCreateInfo {
    const char* vertexShaderPath;
    const char* fragmentShaderPath;
    VkPrimitiveTopology primitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
};

struct Pipeline {
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
};

struct TextureCreateInfo {
    const char* texturePath;
};

struct Texture { 
    VkImageView textureImageView;
    VkSampler textureSampler;
};

struct UniformBuffer {
    std::vector<VkBuffer> uniformBuffers; 
    std::vector<void*> uniformBuffersMapped;
};

struct DescriptorSets {
    std::vector<VkDescriptorSet> descriptorSets;
};


struct CreateModelInfo{
    std::vector<Vertex> punkty;
    std::vector<uint32_t> indeksy;
};


struct DrawModelInfo {
    Model model;
    Transform transform;
    Pipeline pipeline;
    UniformBuffer buffer;
    DescriptorSets descriptorSets;
};

struct DrawPointLightInfo {
    Transform transform;
    Pipeline pipeline;
    UniformBuffer buffer;
    DescriptorSets descriptorSets;
    glm::vec4 color;
};


#define MAX_LIGHTS 100

struct PointLight {
    glm::vec4 position{};
    glm::vec4 color{};
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
    PointLight pointLights[MAX_LIGHTS];
    alignas(4) int numberOfLights;
    alignas(4) glm::float32 iTime;
    alignas(16) glm::vec3 cameraPosition;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


struct DrawAllOfThem {
    std::vector<DrawModelInfo> allModels;
    std::vector<DrawPointLightInfo> allLights;
};

struct SimplePushConstantData {
    glm::vec3 position;
    alignas(16) glm::vec4 color;
};

struct Key {
    bool pressed;
};