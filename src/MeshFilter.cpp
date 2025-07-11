#include "MeshFilter.h"
#include <cstdio>

MeshFilter::MeshFilter(){
    //printf("Just a random number - MeshFilter constructor\n");
}


MeshFilter::MeshFilter(double f){
   // printf("%.2f MeshFilter constructor\n", f);
}

MeshFilter::~MeshFilter(){
    //printf("MeshFilter destructor\n");
}

void MeshFilter::setup(){
    printf("MeshFilter setup\n");
}

void MeshFilter::update(double deltaTime){
    printf("MeshFilter - update deltaTime: %.2f\n", deltaTime);
}