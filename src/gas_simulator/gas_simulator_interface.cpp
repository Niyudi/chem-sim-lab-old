/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include "../config.h"

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
    this->setContentsMargins(0, 0, 0, 0);
    this->setFixedSize(GAS_SIMULATOR_RENDERER_WIDTH, GAS_SIMULATOR_RENDERER_HEIGHT);
}

void GasSimulatorRenderer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    QBrush brush(QColor("grey"), Qt::SolidPattern);
    QPen pen(QColor("grey"), Qt::SolidLine);
    
    painter.setBrush(brush);
    painter.setPen(pen);
    
    painter.drawRect(0, 0, GAS_SIMULATOR_RENDERER_WIDTH - 1, GAS_SIMULATOR_RENDERER_WIDTH - 1);
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