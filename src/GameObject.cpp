#include "GameObject.h"
#include "Component.h"
#include <iostream>
#include <vector>

GameObject::GameObject(){};

GameObject::GameObject(std::vector<Component*> components){
    this->components = components;
    printf("wielkie przypisanie");
};

void GameObject::addComponents(std::vector<Component*> components){
    this->components.insert(this->components.end(), components.begin(), components.end());
};


