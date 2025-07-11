#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/VkDraw.h"

struct Edge{
    int A;
    int B;
    bool isActive;
};

typedef enum EdgeOrientation{
    EDGE_ORIENTATION_SOUTH = 0,
    EDGE_ORIENTATION_NORTH = 1,
    EDGE_ORIENTATION_WEST = 2,
    EDGE_ORIENTATION_EAST = 3,
}EdgeOrientation;

class GridMesh : public VkDraw{
public:
    GridMesh(VulkanEngine* vkEngine, int sizeHorizontal, int sizeVertical, float cellSize, glm::vec3 position);
    DrawModelInfo& getDrawInfo() override;
    Edge* getEdge(int cellX, int cellY, EdgeOrientation edgeOrientation);
    void update(VulkanEngine* vkEngine);
private:
    DrawModelInfo drawInfo;
    std::vector<Vertex> punkty;
    std::vector<Edge> edgesHorizontal;
    std::vector<Edge> edgesVertical;
    int sizeHorizontal;
    int sizeVertical;
    Model getMesh(VulkanEngine* vkEngine, int sizeHorizontal, int sizeVertical, float cellWidth);
};


