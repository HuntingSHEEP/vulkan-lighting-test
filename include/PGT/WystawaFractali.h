#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"

class WystawaFractali{
public:
    WystawaFractali(VulkanEngine* vkEngine);
    DrawModelInfo WystawaFractali::createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform);
    DrawAllOfThem getModels();
private:
    DrawAllOfThem drawAllOfThem;
};