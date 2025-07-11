#pragma once

class Component{
public:
    virtual void setup() = 0;
    virtual void update(double deltaTime) = 0;
};