#pragma once 
#include <vector>
#include <iostream>
#include "Component.h"

class GameObject {
public:
    GameObject();
    GameObject(std::vector<Component*> components);
    ~GameObject();

    void addComponents(std::vector<Component*> components);

    template <typename T> T* getComponent() const{
        for(const auto& component : components){
            if(auto derived = dynamic_cast<T*>(component)){
                return derived;
            }
        }

        return nullptr;
    };

private:
    std::vector<Component*> components;
};