/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator.h"

#include <chrono>
#include <iterator>
#include <math.h>
#include <stdlib.h>
#include <thread>

#include <iostream>
#include <unistd.h>

/*
 * Classes
 */

// GasSimulator

GasSimulator::GasSimulator(QObject* parent) : QThread(parent) {
}

GasSimulator::~GasSimulator() {
    this->kill();
}

int GasSimulator::exec() {
    while (this->active_flag) {
        auto t1 = std::chrono::high_resolution_clock::now(); // Gets starting time
        
        if (this->reset_flag) { // Clears particles_list and randomizes new set of particles
            this->particles_list.clear();
            for (short i = 0 ; i < 200 ; ++i) {
                double* position = new double[2] { rand() / (RAND_MAX / GAS_SIMULATOR_WIDTH), rand() / (RAND_MAX / GAS_SIMULATOR_HEIGHT)};
                double* velocity = new double[2] { -0.1 + (rand() / (RAND_MAX / (0.2))), -0.1 + (rand() / (RAND_MAX / (0.2)))};
                ParticleBody particle = ParticleBody(position, 1.0, 5.0, velocity);
                this->particles_list.push_back(particle);
            }
            this->reset_flag = false;
        }
        
        if (this->running_flag) { // Calculates physics for one frame
            // Updates all positions based on velocities for one frame
            for (auto it = this->particles_list.begin() ; it != this->particles_list.end() ; ++it) {
                it->update(this->FRAME_TIME);
            }
            
            // Detects and resolves collisions
            for (auto it = this->particles_list.begin() ; it != --this->particles_list.end() ; ++it) {
                for (auto it2 = std::next(it) ; it2 != this->particles_list.end() ; ++it2) {
                    // Calculates distance
                    double x = it2->getPosition()[0] - it->getPosition()[0];
                    double y = it2->getPosition()[1] - it->getPosition()[1];
                    double distance_squared = (x * x) + (y * y);
                    
                    //Calculates radii sum
                    double radii_sum_squared = it->getRadius() + it2->getRadius();
                    radii_sum_squared *= radii_sum_squared;
                    
                    if (distance_squared < radii_sum_squared) {
                        it->solveCollision(&(*it2), x, y, sqrt(distance_squared));
                    }
                }
            }
        }
        
        this->frameResults(&this->particles_list); // Emits results signal to renderer
        
        
        auto t2 = std::chrono::high_resolution_clock::now(); // Gets ending time
        float delta = this->FRAME_TIME - (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0); // Calculates deficit in calculation time vs frame time
        
        std::cout << "Delta: " << delta << " / " << this->FRAME_TIME << "\n\n";
        if (delta > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(delta));
        }
    }
    return 0;
}

void GasSimulator::kill() {
    this->active_flag = false;
    this->wait();
}

void GasSimulator::reset() {
    this->reset_flag = true;
}

void GasSimulator::toggle() {
    this->running_flag = !this->running_flag;
}

void GasSimulator::run() {
    this->exec();
}

// ParticleBody

ParticleBody::ParticleBody(double* position, double mass, double radius, double* velocity, double* force)
        : mass(mass), radius(radius), force(force), position(position), velocity(velocity) {
}

double ParticleBody::getMass() const {
    return this->mass;
}

double ParticleBody::getRadius() const {
    return this->radius;
}

double* ParticleBody::getPosition() const {
    return this->position;
}

double* ParticleBody::getVelocity() const {
    return this->velocity;
}

void ParticleBody::solveCollision(ParticleBody* particle, double normal_x, double normal_y, double distance) {
    // Gets other particle's mass
    double inverse_other_mass = particle->getMass();
    double inverse_mass = this->mass;
    
    // Normalizes normal
    normal_x /= distance;
    normal_y /= distance;
    
    // Calculates relative velocity
    double* other_velocity = particle->getVelocity();
    double relative_velocity_x = other_velocity[0] - this->velocity[0];
    double relative_velocity_y = other_velocity[1] - this->velocity[1];
    
    // Calculates projection of velocity in normal
    double velocity_along_normal = (relative_velocity_x * normal_x) + (relative_velocity_y * normal_y);
    
    // Do nothing if the particles are separating
    if (velocity_along_normal > 0) {
        return;
    }
    
    // Calculates magnitude of impulse
    double impulse_scalar = -2 * velocity_along_normal;
    impulse_scalar /= inverse_mass + inverse_other_mass;
    
    // Calculates impulse vector (in normal variables for efficiency)
    normal_x *= impulse_scalar;
    normal_y *= impulse_scalar;
    
    // Applies impulse
    this->velocity[0] -= inverse_mass * normal_x;
    this->velocity[1] -= inverse_mass * normal_y;
    particle->setVelocity(other_velocity[0] + (inverse_other_mass * normal_x), other_velocity[1] + (inverse_other_mass * normal_y));
    
}

void ParticleBody::update(double time) {
    this->position[0] += this->velocity[0] * time;
    this->position[1] += this->velocity[1] * time;
    
    if (this->position[0] < 0) {
        this->position[0] = -this->position[0];
        this->velocity[0] = -this->velocity[0];
    } else if (this->position[0] > GAS_SIMULATOR_WIDTH) {
        this->position[0] = 2 * GAS_SIMULATOR_WIDTH - this->position[0];
        this->velocity[0] = -this->velocity[0];
    }
    if (this->position[1] < 0) {
        this->position[1] = -this->position[1];
        this->velocity[1] = -this->velocity[1];
    } else if (this->position[1] > GAS_SIMULATOR_HEIGHT) {
        this->position[1] = 2 * GAS_SIMULATOR_HEIGHT - this->position[1];
        this->velocity[1] = -this->velocity[1];
    }
}

/*
void ParticleBody::setPosition(double position_x, double position_y) {
    this->position[0] = position_x;
    this->position[1] = position_y;
}
*/

void ParticleBody::setVelocity(double velocity_x, double velocity_y) {
    this->velocity[0] = velocity_x;
    this->velocity[1] = velocity_y;
}