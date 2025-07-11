#pragma once 
#include "Component.h"

class MeshRenderer : public Component{
public:
    MeshRenderer();
    ~MeshRenderer();

    void setup() override;
    void update(double deltaTime) override;
};