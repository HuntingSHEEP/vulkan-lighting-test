#include "GraphicsEngine/GridMesh.h"
#include "GraphicsEngine/GetPointMatrix.h"
#include <iostream>




GridMesh::GridMesh(VulkanEngine* vkEngine, int sizeHorizontal, int sizeVertical, float cellSize, glm::vec3 position){
    this->sizeHorizontal = sizeHorizontal;
    this->sizeVertical = sizeVertical;

    Transform t{};
    t.position = position;
    t.scale = { 1., 1., 1. };
    t.rotationAxis = { 0., 0., 1. };
    t.rotationAngle = 3.1415f;

    Pipeline pipeline = vkEngine->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_LINE_LIST});
    Model gridMesh = getMesh(vkEngine, this->sizeHorizontal, this->sizeVertical, cellSize);
    Texture paper = vkEngine->initTexture({ "../resources/textures/whitePaper2.jpg" });

    UniformBuffer uBuffer = vkEngine->initUniformBuffer();
    DescriptorSets dSet = vkEngine->initDescriptorSet(paper, uBuffer);
    drawInfo = {gridMesh, t, pipeline, uBuffer, dSet};
};

DrawModelInfo& GridMesh::getDrawInfo(){
    return drawInfo;
};

Edge* GridMesh::getEdge(int cellX, int cellY, EdgeOrientation edgeOrientation){
    Edge* edge;

    switch(edgeOrientation){
        case EDGE_ORIENTATION_SOUTH:
            edge = &(edgesHorizontal[cellY*sizeHorizontal + cellX]);
            break;
        case EDGE_ORIENTATION_NORTH:
            edge = &(edgesHorizontal[(cellY+1)*sizeHorizontal + cellX]);
            break;
        case EDGE_ORIENTATION_WEST:
            edge = &(edgesVertical[cellX*sizeVertical + cellY]);
            break;
        case EDGE_ORIENTATION_EAST:
            edge = &(edgesVertical[(cellX+1)*sizeVertical + cellY]);
            break;
    }

    return edge;
}



Model GridMesh::getMesh(VulkanEngine* vkEngine, int sizeH, int sizeV, float cellWidth){
    sizeH++;
    sizeV++;
    
    //TWORZENIE PUNKTóW SIATKI
    for(int x=0; x<sizeH; x++)
        for(int z=0; z<sizeV; z++){
            glm::vec3 point = glm::vec3(x*cellWidth, 0.0, z*cellWidth);
            glm::vec3 color = {0, .99, .82};

            //std::cout<< glm::to_string(point)<<std::endl;
            punkty.push_back({point, color});
    }

    //WYODRĘBNIENIE KRAWĘDZI PIONOWYCH
    for(int x=0; x<sizeH; x++)
        for(int z=0; z<sizeV-1; z++){
            int indeks = x*sizeV + z;
            //std::cout<<indeks<< " -> " <<indeks+1<<std::endl;
            edgesVertical.push_back({indeks, indeks+1, true});
    }

    //WYODRĘBNIENIE KRAWĘDZI POZIOMYCH
    for(int z=0; z<sizeV; z++)
        for(int x=0; x<sizeH-1; x++){
            int indeks = x*sizeV + z;
            //std::cout<<indeks<< " -> " <<indeks+sizeV<<std::endl;
            edgesHorizontal.push_back({indeks, indeks+sizeV, true});
    }


    std::vector<uint32_t> indeksy;
    for(auto e : edgesHorizontal)
        if(e.isActive){
            indeksy.push_back(e.A);        
            indeksy.push_back(e.B);  
        }
        
    for(auto e : edgesVertical)
        if(e.isActive){
            indeksy.push_back(e.A);
            indeksy.push_back(e.B);
        }

    return vkEngine->initModel(punkty, indeksy);
};


void GridMesh::update(VulkanEngine* vkEngine){
    std::vector<uint32_t> indeksy;
    for(auto e : edgesHorizontal)
        if(e.isActive){
            indeksy.push_back(e.A);        
            indeksy.push_back(e.B);  
        }
        
    for(auto e : edgesVertical)
        if(e.isActive){
            indeksy.push_back(e.A);
            indeksy.push_back(e.B);
        }

    vkEngine->updateModel(&(drawInfo.model), punkty, indeksy);
};