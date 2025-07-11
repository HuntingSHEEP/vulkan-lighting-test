#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "TIO/BenchFunction.h"
#include <vector>

inline DrawAllOfThem prepareObjects(VulkanEngine* app) {
    //Pipeline pipeline = app->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv" }); //jamnik
    //Model wall = getPointMatrix(app); //app->initModel({ "../resources/models/wall.obj" });
    //Texture paper = app->initTexture({ "../resources/textures/whitePaper2.jpg" });
   
    BenchFunction* himmelBlau = new BenchFunction(app);

    std::vector<DrawModelInfo> allModels{};
    allModels.push_back(himmelBlau->getDrawInfo());
    
    return { allModels };
}
