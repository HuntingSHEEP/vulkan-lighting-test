#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/VkDraw.h"



class PointMesh : public VkDraw {
public:
    PointMesh(VulkanEngine* app, VkPrimitiveTopology primitiveTopology, CreateModelInfo modelInfo);
    PointMesh(VulkanEngine* app, VkPrimitiveTopology primitiveTopology, int size, float step);
    DrawModelInfo& getDrawInfo() override;
    void update(VulkanEngine* vkEngine);
private:
    DrawModelInfo drawInfo;
};