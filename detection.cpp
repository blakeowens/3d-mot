#include "detection.h"
#include <iostream>

Detection::Detection(int id, Point cp, Point v, float ctime) {
    
    this->id = id;
    this->cp = cp;
    this->v = v;
    this->ctime = ctime;
}

Detection::~Detection() {}

void Detection::info() {

    std::cout << "ID: " << id << std::endl;
    std::cout << "Position: " << "(" << cp.x << ", " << cp.y << ", " << cp.z << ")" << std::endl;
    std::cout << "Velocity: " << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    std::cout << "Time: " << ctime << std::endl << std::endl;
}

int Detection::get_id() {
    
    return id;
}

Point Detection::get_cposition() {

    return cp;
}

Point Detection::get_fposition() {

    return fp;
}

Point Detection::get_velocity() {
    
    return v;
}

float Detection::get_ctime() {

    return ctime;
}

float Detection::get_ftime() {

    return ftime;
}

void Detection::set_id(int id) {
    
    this->id = id;
}

void Detection::set_velocity(Point v) {
    
    this->v = v;
}

void Detection::set_fposition(Point fp) {

    this->fp = fp;
}

void Detection::set_ftime(float ftime) {
    
    this->ftime = ftime;
}