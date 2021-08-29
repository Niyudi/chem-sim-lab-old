/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator.h"

#include <ctime>
#include <stdlib.h>

#include <iostream>
#include <unistd.h>

/*
 * Classes
 */

// GasSimulator

GasSimulator::GasSimulator(QObject* parent) : QThread(parent) {
    srand(time(NULL));
}

GasSimulator::~GasSimulator() {
    this->kill();
}

int GasSimulator::exec() {
    while (this->active_flag) {
        if (this->reset_flag) {
            for (short i = 0 ; i < 50 ; ++i) {
                ParticleBody particle = ParticleBody(new double[2] { rand() / (RAND_MAX / 800.0), rand() / (RAND_MAX / 800.0)}, 5.0);
                this->particles_list.push_back(particle);
            }
            
            std::cout << "Reset!\n";
            this->reset_flag = false;
        }
        
        if (this->running_flag) {
            std::cout << "EXPENSIVE TASK!\n";
        }
        
        usleep(1000000);
    }
}

void GasSimulator::kill() {
    this->active_flag = false;
    this->wait();
}

void GasSimulator::reset() {
    this->reset_flag = true;
}

void GasSimulator::resume() {
    this->running_flag = true;
}

void GasSimulator::run() {
    this->exec();
}

void GasSimulator::stop() {
    this->running_flag = false;
}

// ParticleBody

ParticleBody::ParticleBody(double* position, double radius, double* velocity)
        : radius(radius), position(position), velocity(velocity) {
    
}