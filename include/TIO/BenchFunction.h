#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/VkDraw.h"

class BenchFunction : public VkDraw{
public:
    BenchFunction(VulkanEngine* app);

    void himmelBlauTransformation(Vertex& vertex);
    void normalize(std::vector<Vertex>& vertices, float scale);
    void colorizeByDepth(std::vector<Vertex>& vertices);
    void mapBenchFunction(VulkanEngine* app, Model* model);

    DrawModelInfo& getDrawInfo() override;
private:
    Transform transform;
    DrawModelInfo drawInfo;
};