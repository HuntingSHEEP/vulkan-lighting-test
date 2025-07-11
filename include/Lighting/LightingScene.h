#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"


class LightingScene{
public:
    LightingScene(VulkanEngine* vkEngine);
    DrawAllOfThem getModels();
    DrawModelInfo createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform);
    DrawPointLightInfo createPointLight(VulkanEngine* vkEngine, Pipeline pipeline, Texture texture, Transform transform, glm::vec4 color);
private:
    DrawAllOfThem drawAllOfThem;
};