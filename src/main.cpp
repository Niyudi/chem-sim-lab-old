#include "interface.h"

#include <ctime>

#include <QApplication>

int main(int argc, char* argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);
    
    srand(time(NULL));

    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();

    return app.exec();
}
