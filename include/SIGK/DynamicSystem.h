#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "SIGK/PointMesh.h"

class DynamicSystem{
public:
    DynamicSystem(VulkanEngine* vkEngine, CreateModelInfo modelInfo);
    DynamicSystem(VulkanEngine* vkEngine, CreateModelInfo modelInfo, glm::vec3 p0, float a, float b);
    DynamicSystem(VulkanEngine* vkEngine, glm::vec3 p0, float a, float b);
    DrawModelInfo& getDrawInfo();
    void sequenceTransformation(VulkanEngine* vkEngine, CreateModelInfo modelInfo);
    void sequenceTransformation(VulkanEngine* vkEngine, glm::vec3 p0, float a, float b);
    static void sequenceTransformation(CreateModelInfo* modelInfo, glm::vec3 p0, float a, float b);
    static double f(double a, double x);
private:
    PointMesh* pointMesh;
    Model* model;
};