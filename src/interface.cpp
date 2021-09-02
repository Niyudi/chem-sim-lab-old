#include "interface.h"

#include "config.h"
#include "gas_simulator/gas_simulator_interface.h"

#include <QApplication>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

/*
 * Classes
 */

// HomePage

HomePage::HomePage(QWidget* parent) : QWidget(parent) {
    auto* level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    level0_hbox0->setSpacing(0);
    
    auto* label = new QLabel("Welcome!", this);
    
    level0_hbox0->addStretch(1);
    level0_hbox0->addWidget(label, 0);
    level0_hbox0->addStretch(1);
}

// MainWindow

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Window
    this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    this->setWindowTitle("ChemSimLab");
    
    // Menu
    auto* open_home_page = new QAction("Home", this);
    auto* open_gas_simulator = new QAction("Gas simulator", this);
    auto* quit = new QAction("&Quit", this);

    QMenu* file = menuBar()->addMenu("&File");
    
    file->addAction(open_home_page);
    file->addAction(open_gas_simulator);
    
    file->addSeparator();
    file->addAction(quit);

    connect(open_home_page, &QAction::triggered, this, &MainWindow::openHome);
    connect(open_gas_simulator, &QAction::triggered, this, &MainWindow::openGasSimulator);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    
    // Central widget
    this->setCentralWidget(new HomePage(this));
}

void MainWindow::openHome() {
    if (this->page == HOME_PAGE) {
        return;
    } else {
        this->page = HOME_PAGE;
        delete this->centralWidget();
        this->setCentralWidget(new HomePage(this));
    }
}

void MainWindow::openGasSimulator() {
    if (this->page == GAS_SIMULATOR_PAGE) {
        return;
    } else {
        this->page = GAS_SIMULATOR_PAGE;
        delete this->centralWidget();
        this->setCentralWidget(new GasSimulatorWidget(this));
    }
}