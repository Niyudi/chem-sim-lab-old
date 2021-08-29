/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "interface.h"

#include "config.h"
#include "gas_simulator/gas_simulator_interface.h"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

/*
 * Classes
 */

// MainWindow

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Window
    this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    this->setWindowTitle("ChemSimLab");
    
    // Menu
    auto* quit = new QAction("&Quit", this);

    QMenu* file = menuBar()->addMenu("&File");
    file->addAction(quit);

    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    
    // Central widget
    auto* gas_simulator_widget = new GasSimulatorWidget(this);
    setCentralWidget(gas_simulator_widget);
}