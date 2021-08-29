/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: niyudi
 *
 * Created on 26 de agosto de 2021, 15:34
 */

#include "interface.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char* argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();

    return app.exec();
}
