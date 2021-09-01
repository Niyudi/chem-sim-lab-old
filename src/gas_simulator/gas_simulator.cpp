#include "gas_simulator.h"

#include <chrono>
#include <iterator>
#include <math.h>
#include <stdlib.h>
#include <thread>

// Debugger :)
#include <iostream>

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
        
        if (this->reset_flag) { // Randomizes new particles_list
            this->particle_number = this->new_particle_number;
            
            delete this->particles_list;
            this->particles_list = new ParticleBody* [this->particle_number];
            
            for (short i = 0 ; i < this->particle_number ; ++i) {
                double* position = new double[2] { rand() / (RAND_MAX / GAS_SIMULATOR_WIDTH), rand() / (RAND_MAX / GAS_SIMULATOR_HEIGHT)};
                double* velocity = new double[2] { -0.1 + (rand() / (RAND_MAX / (0.2))), -0.1 + (rand() / (RAND_MAX / (0.2)))};
                this->particles_list[i] = new ParticleBody(position, 1.43, 3.0, velocity);
            }
            
            this->acumulated_frame_time = 0.0;
            this->frame_count = 1;
            
            this->reset_flag = false;
        }
        
        if (this->running_flag) { // Calculates physics for one frame
            // Updates all positions based on velocities for one frame
            for (short i = 0 ; i < this->particle_number ; ++i) {
                this->particles_list[i]->update(this->MAX_FRAME_TIME);
            }
            
            // Detects and resolves collisions
            
            for (short i1 = 0 ; i1 < this->particle_number - 1 ; ++i1) {
                for (short i2 = i1 + 1 ; i2 < this->particle_number ; ++i2) {
                    // Calculates distance
                    double x = this->particles_list[i2]->getPosition()[0] - this->particles_list[i1]->getPosition()[0];
                    double y = this->particles_list[i2]->getPosition()[1] - this->particles_list[i1]->getPosition()[1];
                    double distance_squared = (x * x) + (y * y);
                    
                    //Calculates radii sum
                    double radii_sum_squared = this->particles_list[i1]->getRadius() + this->particles_list[i2]->getRadius();
                    radii_sum_squared *= radii_sum_squared;
                    
                    if (distance_squared < radii_sum_squared) {
                        this->particles_list[i1]->solveCollision(this->particles_list[i2], x, y, sqrt(distance_squared));
                    }
                }
            }
            
            ++this->frame_count;
        }
        
        this->particlesFrameResults(this->particles_list, this->particle_number); // Emits results signal to renderer
        
        auto t2 = std::chrono::high_resolution_clock::now(); // Gets ending time
        double frame_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0; // Calculates frame time in milliseconds
        
        this->acumulated_frame_time += frame_time;
        if (this->frame_count % 60 == 0) {
            this->frameTimeData(this->acumulated_frame_time / 60.0, this->MAX_FRAME_TIME); // Emits frame time data signal to widget
            this->acumulated_frame_time = 0.0;
        }
        
        float delta = this->MAX_FRAME_TIME - frame_time; // Calculates difference between max frame time vs frame time
        if (delta > 0) { // Sleeps for excess time
            std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(delta));
        }
    }
    return 0;
}

void GasSimulator::kill() {
    this->active_flag = false;
    this->wait();
}

void GasSimulator::reset(int particle_number) {
    this->new_particle_number = particle_number; // Changes particle number
    
    this->reset_flag = true;
}

void GasSimulator::toggle() {
    this->running_flag = !this->running_flag;
}

void GasSimulator::run() {
    this->exec();
}

// ParticleBody

ParticleBody::ParticleBody(double* position, double mass, double radius, double* velocity)
        : mass(mass), radius(radius), position(position), velocity(velocity), effective_height(GAS_SIMULATOR_HEIGHT - radius), effective_width(GAS_SIMULATOR_WIDTH - radius), inverse_mass(1.0 / mass) {
}

ParticleBody::~ParticleBody() {
    delete this->position;
    delete this->velocity;
}

double ParticleBody::getInverseMass() const {
    return this->inverse_mass;
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
    // Gets some data
    double inverse_mass = this->inverse_mass;
    double inverse_other_mass = particle->getInverseMass();
    double* other_velocity = particle->getVelocity();
    
    // Normalizes normal
    normal_x /= distance;
    normal_y /= distance;
    
    // Calculates relative velocity
    double relative_velocity_x = other_velocity[0] - this->velocity[0];
    double relative_velocity_y = other_velocity[1] - this->velocity[1];
    
    // Calculates projection of velocity in normal
    double velocity_along_normal = (relative_velocity_x * normal_x) + (relative_velocity_y * normal_y);
    
    // Do nothing if the particles are separating
    if (velocity_along_normal > 0) {
        return;
    }
    
    // Calculates magnitude of impulse
    double impulse_scalar = (-2 * velocity_along_normal) / (inverse_mass + inverse_other_mass);
    
    // Calculates impulse vector (in normal variables for efficiency)
    double impulse_x = normal_x * impulse_scalar;
    double impulse_y = normal_y * impulse_scalar;
    
    // Applies impulse
    this->velocity[0] -= inverse_mass * impulse_x;
    this->velocity[1] -= inverse_mass * impulse_y;
    particle->setVelocity(other_velocity[0] + (inverse_other_mass * impulse_x), other_velocity[1] + (inverse_other_mass * impulse_y));
    
}

void ParticleBody::update(double time) {
    this->position[0] += this->velocity[0] * time;
    this->position[1] += this->velocity[1] * time;
    
    if (this->position[0] < this->radius) {
        this->position[0] = 2 * this->radius - this->position[0];
        this->velocity[0] = -this->velocity[0];
    } else if (this->position[0] > this->effective_width) {
        this->position[0] = 2 * this->effective_width - this->position[0];
        this->velocity[0] = -this->velocity[0];
    }
    if (this->position[1] < this->radius) {
        this->position[1] = 2 * this->radius - this->position[1];
        this->velocity[1] = -this->velocity[1];
    } else if (this->position[1] > this->effective_height) {
        this->position[1] = 2 * this->effective_height - this->position[1];
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