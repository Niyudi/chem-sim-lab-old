/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gas_simulator.h
 * Author: niyudi
 *
 * Created on 27 de agosto de 2021, 16:22
 */

#ifndef GAS_SIMULATOR_H
#define GAS_SIMULATOR_H

class GasSimulator {
public:
    GasSimulator();
    
    void reset();
    void run();
    void start();
    void stop();
private:
    bool active_flag = false;
    bool reset_flag = true;
    bool running_flag = false;
};

class ParticleBody {
public:
    ParticleBody(double* position, double radius, double* velocity = new double[2] {0.0, 0.0});
private:
    double radius;
    
    double* position;
    double* velocity;
};

#endif /* GAS_SIMULATOR_H */

