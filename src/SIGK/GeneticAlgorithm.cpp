#include "SIGK/GeneticAlgorithm.h"
#include "GraphicsEngine/GetPointMatrix.h"

#include <random>
#include <ctime>




GeneticAlgorithm::GeneticAlgorithm(VulkanEngine* vkEngine, GeneticAlgorithmParams params){
    srand(static_cast<unsigned int>(time(nullptr)));
    this->params = params;
    currentGeneration = 0;

    //CreateModelInfo modelInfo = getPointMatrix(100, .01f);
    //goalSystem = new DynamicSystem(vkEngine, modelInfo, params.startPoint, params.a, params.b);

    CreateModelInfo modelInfo = getPointMatrix(params.meshSize, params.meshDensity);
    DynamicSystem::sequenceTransformation(&modelInfo, params.startPoint, params.a, params.b);
    goalSystem = new DynamicSystem(vkEngine, modelInfo);

    modelInfo = getPointMatrix(params.meshSize, params.meshDensity);
    currentBest = new DynamicSystem(vkEngine, modelInfo);
    

    for(int i=0; i<params.populationSize; i++){
        CreateModelInfo memberModelInfo = getPointMatrix(params.meshSize, params.meshDensity);
        float a, b, fitness;

        do{
            a = (static_cast<float>(rand()) / RAND_MAX) -0.7f;
            b = (static_cast<float>(rand()) / RAND_MAX) / 5.f;
            DynamicSystem::sequenceTransformation(&memberModelInfo, params.startPoint, a, b);

            fitness = getFitnessLevel(memberModelInfo);
        }while((fitness < 0.) || (5. < fitness));

        PopulationMember member = {memberModelInfo, fitness, a, b};
        population.push_back(member);
    }

    printf("THE STARTING RANDOM GENERATION ************************ \n");
    for(auto p : population)
        printf(" a,b : (%5.5f, %5.5f)  FITNESS: %5.5f\n", p.a, p.b, p.fitnessLevel);
    setCurrentBest(vkEngine);
    printf("************************ THE STARTING RANDOM GENERATION  \n\n\n\n");

};

DrawModelInfo& GeneticAlgorithm::getDrawInfo(){
    return currentBest->getDrawInfo();
};

void GeneticAlgorithm::update(VulkanEngine* vkEngine){

    if(currentGeneration < params.generationsCount){
        std::vector<PopulationMember> nextGeneration;

        while(nextGeneration.size() < params.populationSize){
            std::vector<PopulationMember> drawnMembers = drawMember(population);
            
            crossOver(&drawnMembers[0], &drawnMembers[1]);

            DynamicSystem::sequenceTransformation(&(drawnMembers[0].modelInfo), params.startPoint, drawnMembers[0].a, drawnMembers[0].b);
            drawnMembers[0].fitnessLevel = getFitnessLevel(drawnMembers[0].modelInfo);

            DynamicSystem::sequenceTransformation(&(drawnMembers[1].modelInfo), params.startPoint, drawnMembers[1].a, drawnMembers[1].b);
            drawnMembers[1].fitnessLevel = getFitnessLevel(drawnMembers[1].modelInfo);

            nextGeneration.push_back(drawnMembers[0]);
            nextGeneration.push_back(drawnMembers[1]);
        }

        population = nextGeneration;
        currentGeneration++;

        for(auto p : population)
            printf(" a,b : (%5.5f, %5.5f)  FITNESS: %5.5f\n", p.a, p.b, p.fitnessLevel);

        setCurrentBest(vkEngine);
    }
    
};

void GeneticAlgorithm::setCurrentBest(VulkanEngine* vkEngine){
    int index = 0;
    float fit = population[index].fitnessLevel;

    for(int i=1; i<population.size(); i++)
        if(population[i].fitnessLevel < fit){
            index = i;
            fit = population[index].fitnessLevel;
        }

    printf("\n Generation best fit: %5.5f  [%d] a,b (%5.5f, %5.5f)\n\n\n", fit, index, population[index].a, population[index].b);

    currentBest->sequenceTransformation(vkEngine, population[index].modelInfo);
};



std::vector<PopulationMember> GeneticAlgorithm::drawMember(std::vector<PopulationMember> population) {
    // Oblicz całkowitą sumę odwrotności fitnessLevel
    float totalFitness = 0.0f;
    for (const auto& member : population) {
        //printf("Fitness: %5.5f\n", member.fitnessLevel);
        totalFitness += 1.0f / (member.fitnessLevel + 1e-6f); // Dodanie małej wartości, aby uniknąć dzielenia przez zero
    }

    // Funkcja pomocnicza do losowania jednego członka
    auto drawOne = [&](float totalFitness) -> PopulationMember {
        float randomValue = static_cast<float>(rand()) / RAND_MAX * totalFitness;
        float cumulativeFitness = 0.0f;

        for (const auto& member : population) {
            cumulativeFitness += 1.0f / (member.fitnessLevel + 1e-6f);
            if (cumulativeFitness >= randomValue) {
                return member;
            }
        }
        return population.back(); // W przypadku, gdy losowanie nie zadziała poprawnie, zwróć ostatniego członka
    };

    // Losuj dwóch członków
    PopulationMember member1 = drawOne(totalFitness);
    PopulationMember member2 = drawOne(totalFitness);

    //printf("(%2.4f, %2.4f) oraz (%2.4f, %2.4f)\n", member1.a, member1.b, member2.a, member2.b);

    return {member1, member2};
}


void GeneticAlgorithm::crossOver(PopulationMember* m1, PopulationMember* m2) {
    srand(static_cast<unsigned int>(time(nullptr)));
    float a1 = m1->a;
    float a2 = m2->a;
    float b1 = m1->b;
    float b2 = m2->b;

    if((static_cast<float>(rand()) / RAND_MAX) < 0.5f){
        m1->a = a1;
        m1->b = b2;
        m2->a = a2;
        m2->b = b1;
    } else {
        m1->a = a1;
        m1->b = a2;
        m2->a = b1;
        m2->b = b2;
    }


    if((static_cast<float>(rand()) / RAND_MAX) < params.pMutation){
        float r = (static_cast<float>(rand()) / RAND_MAX);
        m1->a -= r;
        m2->b -= r;
    }

};


float GeneticAlgorithm::getFitnessLevel(CreateModelInfo memberModelInfo) {
    double level = 0.0;

    for (int i = 0; i < memberModelInfo.punkty.size(); i++) {
        glm::vec3 goalVertex = goalSystem->getDrawInfo().model.vertices[i].pos;
        glm::vec3 memberVertex = memberModelInfo.punkty[i].pos;

        level += glm::distance(goalVertex, memberVertex);
    }

    level /= (memberModelInfo.punkty.size());
    return static_cast<float>(level);
}