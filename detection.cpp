#include "detection.h"
#include <iostream>

Detection::Detection(int id, Point p, Point v, float time) {
    
    this->id = id;
    this->p = p;
    this->v = v;
    this->time = time;
}

Detection::~Detection() {}

void Detection::info() {

    std::cout << "ID: " << id << std::endl;
    std::cout << "Position: " << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    std::cout << "Velocity: " << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    std::cout << "Time: " << time << std::endl;
}

int Detection::getID() {
    return id;
}