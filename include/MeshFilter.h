#pragma once 
#include "Component.h"

class MeshFilter : public Component{
public: 
    MeshFilter();
    MeshFilter(double f);
    ~MeshFilter();

    void setup() override;
    void update(double deltaTime) override;
};