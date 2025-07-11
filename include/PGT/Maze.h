#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/GridMesh.h"

typedef enum MazeBuildAlgorithm{
    MAZE_BUILD_ALGORITHM_BINARY_TREE = 0,
    MAZE_BUILD_ALGORITHM_SIDEWINDER = 1,
}MazeBuildAlgorithm;

class Maze{
public:
    Maze(VulkanEngine* vkEngine, int sizeHorizontal, int sizeVertical, float cellSize, MazeBuildAlgorithm MazeBuildAlgorithm, glm::vec3 position);
    DrawModelInfo& getDrawInfo();

    void generateMazeBinaryTree(int sizeHorizontal, int sizeVertical);
    void generateMazeSidewinder(int sizeHorizontal, int sizeVertical);
private:    
    GridMesh* gridMesh;
};