#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>

// Enums

enum Pages {
    HOME_PAGE,
    GAS_SIMULATOR_PAGE
};

// Classes

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
public slots:
    void openHome();
    void openGasSimulator();
private:
    Pages page = HOME_PAGE;
};

class HomePage : public QWidget {
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);;
};


#endif /* INTERFACE_H */

