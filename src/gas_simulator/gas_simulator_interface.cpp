/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include "../config.h"

#include <ctime> 

#include <QBrush>
#include <QColor>
#include <QHBoxLayout>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

// GasSimulatorRenderer

GasSimulatorRenderer::GasSimulatorRenderer(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(GAS_SIMULATOR_RENDERER_WIDTH, GAS_SIMULATOR_RENDERER_HEIGHT);
    
    srand(time(NULL));
    
    for (short i = 0 ; i < 100 ; ++i) {
        ParticleImage particle = {.position = {rand() % 800, rand() % 800}, .radius = 5};
        this->particles_list.push_back(particle);
    }
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

GasSimulatorWidget::GasSimulatorWidget(QWidget *parent) : QWidget(parent) {   
    this->initUI();
}

void GasSimulatorWidget::initUI() {
    /*
     * Widgets
     */
    
    // Push buttons
    
    auto *start_button = new QPushButton("Start simulation");
    
    // Renderer
    
    auto *renderer = new GasSimulatorRenderer();
    
    /*
     * Layouts
     */
    
    auto *level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    level0_hbox0->setSpacing(0);
    
    auto *level1_vbox0 = new QVBoxLayout();
    
    /*
     * Strucutre
     */
    
    level0_hbox0->addLayout(level1_vbox0, 1);
    level0_hbox0->addWidget(renderer, 0);
    
    level1_vbox0->addWidget(start_button, 0);
}