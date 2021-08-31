/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include "../config.h"
#include "gas_simulator.h"

#include <ctime>
#include <math.h>

#include <QBrush>
#include <QColor>
#include <QHBoxLayout>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

/*
 * Classes
 */

// GasSimulatorRenderer

GasSimulatorRenderer::GasSimulatorRenderer(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(round(GAS_SIMULATOR_WIDTH), round(GAS_SIMULATOR_HEIGHT));
    
    // Initiates gas simulator thread
    this->simulator = new GasSimulator(this);
    connect(this->simulator, &GasSimulator::frameResults, this, &GasSimulatorRenderer::update);
    connect(this->simulator, &GasSimulator::finished, this->simulator, &QObject::deleteLater);
    this->simulator->start();
}

GasSimulator* GasSimulatorRenderer::getSimulator() const {
    return this->simulator;
}

void GasSimulatorRenderer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    // Colors
    QColor brown(117, 75, 11);
    QColor light_yellow(250, 250, 230);
    
    // Initialize painter
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    
    // Paint
    brush.setColor(light_yellow);
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(light_yellow);
    pen.setStyle(Qt::SolidLine);
    
    painter.setBrush(brush);
    painter.setPen(pen);
    
    painter.drawRect(0, 0, GAS_SIMULATOR_WIDTH, GAS_SIMULATOR_WIDTH);
    
    brush.setColor(brown);
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(brown);
    pen.setStyle(Qt::SolidLine);
    
    painter.setBrush(brush);
    painter.setPen(pen);
    
    for (auto it = this->particles_list.begin() ; it != this->particles_list.end() ; ++it) {
        painter.drawEllipse(it->position[0], it->position[1], 2 * it->radius, 2 * it->radius);
    }
}

void GasSimulatorRenderer::update(std::vector<ParticleBody>* particle_bodies_list) {
    this->particles_list.clear();
    
    for (auto it = particle_bodies_list->begin() ; it != particle_bodies_list->end() ; ++it) {
        short adjusted_x = round(it->getPosition()[0] - it->getRadius());
        short adjusted_y = round(it->getPosition()[1] - it->getRadius());
        ParticleImage particle = {.position = {adjusted_x, adjusted_y}, .radius = round(it->getRadius())};
        this->particles_list.push_back(particle);
    }
    
    this->repaint();
}

// GasSimulatorWidget

GasSimulatorWidget::GasSimulatorWidget(QWidget* parent) : QWidget(parent) {   
    this->initUI();
}

void GasSimulatorWidget::initUI() {
    /*
     * Widgets
     */
    
    // Push buttons
    
    auto* reset_button = new QPushButton("Reset simulation");
    auto* toggle_button = new ToggleGasSimulatorButton();
    
    // Renderer
    
    auto* renderer = new GasSimulatorRenderer();
    
    /*
     * Layouts
     */
    
    auto* level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    level0_hbox0->setSpacing(0);
    
    auto* level1_vbox0 = new QVBoxLayout();
    
    auto* level2_hbox0 = new QHBoxLayout();
    
    /*
     * Strucutre
     */
    
    level0_hbox0->addLayout(level1_vbox0, 1);
    level0_hbox0->addWidget(renderer, 0);
    
    level1_vbox0->addStretch(1);
    level1_vbox0->addLayout(level2_hbox0, 0);
    level1_vbox0->addStretch(1);
    
    level2_hbox0->addWidget(toggle_button, 0);
    level2_hbox0->addWidget(reset_button, 0);
    
    /*
     * Signals
     */
    
    QObject::connect(reset_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::reset);
    QObject::connect(toggle_button, &ToggleGasSimulatorButton::clicked, renderer->getSimulator(), &GasSimulator::toggle);
}

// ToggleGasSimulatorButton

ToggleGasSimulatorButton::ToggleGasSimulatorButton(QWidget* parent) : QPushButton(parent) {
    this->setText("Resume simulation");
    connect(this, &ToggleGasSimulatorButton::clicked, this, &ToggleGasSimulatorButton::toggleLabel);
}

void ToggleGasSimulatorButton::toggleLabel() {
    if (this->text() == "Resume simulation") {
        this->setText("Stop simulation");
    } else {
        this->setText("Resume simulation");
    }
}