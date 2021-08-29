/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include "../config.h"
#include "gas_simulator.h"

#include <ctime> 

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
    this->setFixedSize(GAS_SIMULATOR_RENDERER_WIDTH, GAS_SIMULATOR_RENDERER_HEIGHT);
    
    // Initiates gas simulator thread
    this->simulator = new GasSimulator(this);
    connect(this->simulator, &GasSimulator::finished, this->simulator, &QObject::deleteLater);
    this->simulator->start();
    
    srand(time(NULL));
    
    for (short i = 0 ; i < 100 ; ++i) {
        ParticleImage particle = {.position = {rand() % 800, rand() % 800}, .radius = 5};
        this->particles_list.push_back(particle);
    }
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
    
    painter.drawRect(0, 0, GAS_SIMULATOR_RENDERER_WIDTH - 1, GAS_SIMULATOR_RENDERER_WIDTH - 1);
    
    brush.setColor(brown);
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(brown);
    pen.setStyle(Qt::SolidLine);
    
    painter.setBrush(brush);
    painter.setPen(pen);
    
    for (auto it = this->particles_list.begin() ; it != this->particles_list.end() ; ++it) {
        painter.drawEllipse(it->position[0], it->position[1], it->radius, it->radius);
    }
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
    auto* resume_button = new QPushButton("Resume simulation");
    auto* stop_button = new QPushButton("Stop simulation");
    
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
    level1_vbox0->addWidget(reset_button, 0);
    level1_vbox0->addLayout(level2_hbox0, 0);
    level1_vbox0->addStretch(1);
    
    level2_hbox0->addWidget(resume_button, 0);
    level2_hbox0->addWidget(stop_button, 0);
    
    /*
     * Signals
     */
    
    QObject::connect(reset_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::reset);
    QObject::connect(resume_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::resume);
    QObject::connect(stop_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::stop);
}