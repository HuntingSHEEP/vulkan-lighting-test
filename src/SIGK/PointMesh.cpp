#include "SIGK/PointMesh.h"
#include "GraphicsEngine/GetPointMatrix.h"
#include <iostream>

PointMesh::PointMesh(VulkanEngine* app, VkPrimitiveTopology primitiveTopology, CreateModelInfo modelInfo){
    Transform t{};
    t.position = { 0., 0., 0. };
    t.scale = { 1., 1., 1. };
    t.rotationAxis = { 0., 0., 1. };
    t.rotationAngle = 3.1415f;

    Pipeline pipeline = app->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", primitiveTopology});
    Model wall = getPointMatrix(app, modelInfo);
    Texture paper = app->initTexture({ "../resources/textures/whitePaper2.jpg" });

    UniformBuffer uBuffer = app->initUniformBuffer();
    DescriptorSets dSet = app->initDescriptorSet(paper, uBuffer);
    drawInfo = {wall, t, pipeline, uBuffer, dSet};
};

PointMesh::PointMesh(VulkanEngine* app, VkPrimitiveTopology primitiveTopology, int size, float step){
    Transform t{};
    t.position = { 0., 0., 0. };
    t.scale = { 1., 1., 1. };
    t.rotationAxis = { 0., 0., 1. };
    t.rotationAngle = 3.1415f;

    Pipeline pipeline = app->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", primitiveTopology});
    Model wall = getPointMatrix(app, size, step);
    Texture paper = app->initTexture({ "../resources/textures/whitePaper2.jpg" });

    UniformBuffer uBuffer = app->initUniformBuffer();
    DescriptorSets dSet = app->initDescriptorSet(paper, uBuffer);
    drawInfo = {wall, t, pipeline, uBuffer, dSet};
};

DrawModelInfo& PointMesh::getDrawInfo(){
    return drawInfo;
}

void PointMesh::update(VulkanEngine* vkEngine){
    std::vector<Vertex> vertices = drawInfo.model.vertices;

    for(auto& v: vertices)
        v.color = {.0, .5, 0.9};

    vkEngine->updateModel(&(drawInfo.model), vertices);
};