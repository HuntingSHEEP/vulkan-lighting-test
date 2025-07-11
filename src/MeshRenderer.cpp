#include "MeshRenderer.h"
#include <cstdio> 

MeshRenderer::MeshRenderer(){
  //  printf("mesh renderer constructor\n");
};

MeshRenderer::~MeshRenderer(){
  //  printf("mesh renderer destructor\n");
}

void MeshRenderer::setup(){
    printf("mesh rendererr setup\n");
}

void MeshRenderer::update(double deltaTime){
    printf("mesh renderer update: %.2f\n", deltaTime);
}