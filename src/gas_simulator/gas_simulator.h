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

#include "../config.h"

#include <vector>

#include <QThread>

// Forward declarations

class ParticleBody;

// Classes

class GasSimulator : public QThread {
    Q_OBJECT
public:
    GasSimulator(QObject* parent = nullptr);
    ~GasSimulator();
    
    void kill();
public slots:
    void reset();
    void resume();
    void stop();
protected:   
    void run() override;
    
    int exec();
signals:
    void frameResults(std::vector<ParticleBody>* particle_bodies_list);
private:
    std::vector<ParticleBody> particles_list;
    
    bool active_flag = true;
    bool reset_flag = true;
    bool running_flag = false;
    
    static constexpr float frame_time = 1.0 / FPS;
};

class ParticleBody {
public:
    ParticleBody(double* position, double radius, double* velocity = new double[2] {0.0, 0.0});
    
    double getRadius() const;
    
    double* getPosition() const;
    //double* getVelocity() const;
private:
    double radius;
    
    double* position;
    double* velocity;
};

#endif /* GAS_SIMULATOR_H */

