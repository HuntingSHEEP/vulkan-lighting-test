#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include <vector>
#include <iostream>


inline CreateModelInfo getPointMatrix(int size, float step) {
    std::vector<Vertex> punkty;
    std::vector<uint32_t> indeksy;

    for(int x=0; x<size; x++)
        for (int z = 0; z < size; z++) {
            Vertex v = { {x*step, 0.0, z*step}, {1, 0.1, 1} };
            punkty.push_back(v);
            indeksy.push_back(x*size + z);
        }

    return {punkty, indeksy};
}


inline Model getPointMatrix(VulkanEngine* app, CreateModelInfo modelInfo) {
    return app->initModel(modelInfo.punkty, modelInfo.indeksy);
}

inline Model getPointMatrix(VulkanEngine* app, int size, float step) {
    CreateModelInfo modelInfo = getPointMatrix(size, step);
    return app->initModel(modelInfo.punkty, modelInfo.indeksy);
}



inline Model getPointMatrix(VulkanEngine* app, float A, float B, int n) {
    std::vector<Vertex> punkty;
    std::vector<uint32_t> indeksy;
    
    float wymiar = abs(B-A);
    float skok = wymiar/n;
    glm::vec3 onlyA = {A, .0, A};

    for(int q=0; q<n; q++)
        for (int i = 0; i < n; i++) {
            glm::vec3 middle = glm::vec3(q*skok, 0.0, i * skok) + onlyA;
            glm::vec3 color = {0, .99, .2};
            Vertex v = { middle, color };
            int index = q*n + i;

            //std::cout<<index << "  " << glm::to_string(v.pos) <<std::endl;
            punkty.push_back(v);
            indeksy.push_back(index);
        }
    
    return app->initModel(punkty, indeksy);
}
