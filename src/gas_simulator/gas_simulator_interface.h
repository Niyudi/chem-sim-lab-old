/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gas_simulator_interface.h
 * Author: niyudi
 *
 * Created on 26 de agosto de 2021, 22:22
 */

#ifndef GAS_SIMULATOR_INTERFACE_H
#define GAS_SIMULATOR_INTERFACE_H

#include "gas_simulator.h"

#include <vector>

#include <QWidget>

// Structs

struct ParticleImage {
    short position [2];
    short radius;
};

// Classes

class GasSimulatorRenderer : public QWidget {
    Q_OBJECT
public:
    GasSimulatorRenderer(QWidget* parent = nullptr);
    
    GasSimulator* getSimulator() const;
protected:
    void paintEvent(QPaintEvent* event);
private:
    std::vector<ParticleImage> particles_list;
    
    GasSimulator* simulator;
};

class GasSimulatorWidget : public QWidget {
    Q_OBJECT
public:
    GasSimulatorWidget(QWidget* parent = nullptr);
private:
    void initUI();
};

#endif /* GAS_SIMULATOR_INTERFACE_H */

