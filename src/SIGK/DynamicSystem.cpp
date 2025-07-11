#include "SIGK/DynamicSystem.h"

DynamicSystem::DynamicSystem(VulkanEngine* vkEngine, CreateModelInfo modelInfo){
    pointMesh = new PointMesh(vkEngine, VK_PRIMITIVE_TOPOLOGY_POINT_LIST, modelInfo);
    model = &(pointMesh->getDrawInfo().model);
};

DynamicSystem::DynamicSystem(VulkanEngine* vkEngine, CreateModelInfo modelInfo, glm::vec3 p0, float a, float b){
    pointMesh = new PointMesh(vkEngine, VK_PRIMITIVE_TOPOLOGY_POINT_LIST, modelInfo);
    model = &(pointMesh->getDrawInfo().model);
    sequenceTransformation(vkEngine, p0, a, b);
};

DynamicSystem::DynamicSystem(VulkanEngine* vkEngine, glm::vec3 p0, float a, float b){
    pointMesh = new PointMesh(vkEngine, VK_PRIMITIVE_TOPOLOGY_POINT_LIST, 100, .01f);
    model = &(pointMesh->getDrawInfo().model);
    sequenceTransformation(vkEngine, p0, a, b);
};

DrawModelInfo& DynamicSystem::getDrawInfo(){
    return pointMesh->getDrawInfo();
};

void DynamicSystem::sequenceTransformation(CreateModelInfo* modelInfo, glm::vec3 p0, float a, float b){
    glm::vec3 prevPoint = p0;
    modelInfo->punkty[0].pos = {p0.x, p0.z, p0.y};

    for (int i = 1; i < modelInfo->punkty.size(); i++) {
        double xn = prevPoint.y + b * (1 - 0.05 * prevPoint.y * prevPoint.y) * prevPoint.y + DynamicSystem::f(prevPoint.x, a);
        double yn = -prevPoint.x + DynamicSystem::f(xn, a);
        double zn = prevPoint.z;

        prevPoint = { xn, yn, zn};
        modelInfo->punkty[i].pos = prevPoint; //{ xn, zn, yn};
    }

    for (auto& v : modelInfo->punkty){      
        v.color = {1., 1., .2};  
        v.pos.z = v.pos.y;
        v.pos.y = 0.f;
    }
};

void DynamicSystem::sequenceTransformation(VulkanEngine* vkEngine, glm::vec3 p0, float a, float b) {
    CreateModelInfo modelInfo = {model->vertices};
    DynamicSystem::sequenceTransformation(&modelInfo, p0, a, b);

    vkEngine->updateModel(model, modelInfo.punkty);
};

void DynamicSystem::sequenceTransformation(VulkanEngine* vkEngine, CreateModelInfo modelInfo) {
    vkEngine->updateModel(model, modelInfo.punkty);
};

double DynamicSystem::f(double a, double x) {
    return a * x + (2 * (1 - a) * x * x) / (1 - x + x * x);
};