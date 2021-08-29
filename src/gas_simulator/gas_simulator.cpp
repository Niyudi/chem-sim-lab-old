/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator.h"

#include <ctime>

/*
 * Classes
 */

// GasSimulator

GasSimulator::GasSimulator() {
    
}

void GasSimulator::run() {
    this->active_flag = true;
    while (this->active_flag) {
        
    }
}

// ParticleBody

ParticleBody::ParticleBody(double* position, double radius, double* velocity)
        : radius(radius), position(position), velocity(velocity) {
    
}