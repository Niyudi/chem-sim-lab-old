/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

// GasSimulatorWidget

GasSimulatorWidget::GasSimulatorWidget(QWidget *parent) : QWidget(parent) {
    this->initUI();
}

void GasSimulatorWidget::initUI() {
    /*
     * Widgets
     */
    
    auto *start_button = new QPushButton("Start simulation");
    
    /*
     * Layouts
     */
    
    auto *level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setSpacing(0);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    
    auto *level1_vbox0 = new QVBoxLayout();
    
    /*
     * Strucutre
     */
    
    level0_hbox0->addLayout(level1_vbox0, 1);
    
    level1_vbox0->addWidget(start_button, 0);
}