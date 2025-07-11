#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "SIGK/DynamicSystem.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <bitset>
#include <random>

struct GeneticAlgorithmParams{
    float a;
    float b;
    glm::vec3 startPoint;
    float pCrossOver;
    float pMutation;
    int populationSize;
    int generationsCount;
    int meshSize;
    float meshDensity;

    void init(){
        srand(static_cast<unsigned int>(time(nullptr)));
    };

    void generateRandomParams(){
        a = (static_cast<float>(rand()) / RAND_MAX)*2 -1;
        b = (static_cast<float>(rand()) / RAND_MAX)*2 -1;
    };
};

struct PopulationMember{
    CreateModelInfo modelInfo;
    float fitnessLevel;
    float a;
    float b;
};

class GeneticAlgorithm{
public:
    GeneticAlgorithm(VulkanEngine* vkEngine, GeneticAlgorithmParams params);
    DrawModelInfo& getDrawInfo();
    void update(VulkanEngine* vkEngine);
    float getFitnessLevel(CreateModelInfo memberModelInfo);
    std::vector<PopulationMember> drawMember(std::vector<PopulationMember> population);
    void crossOver(PopulationMember* m1, PopulationMember* m2);
    void setCurrentBest(VulkanEngine* vkEngine);
private:
    GeneticAlgorithmParams params;
    DynamicSystem *goalSystem, *currentBest;
    std::vector<PopulationMember> population;
    int currentGeneration;  
};