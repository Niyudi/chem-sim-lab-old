#ifndef GAS_SIMULATOR_INTERFACE_H
#define GAS_SIMULATOR_INTERFACE_H

#include "gas_simulator.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

// Structs

struct ParticleImage {
    short position [2];
    short radius;
};

// Classes

class GasSimulatorRenderer : public QWidget {
    Q_OBJECT
public:
    GasSimulatorRenderer(QWidget* parent = nullptr);
    
    GasSimulator* getSimulator() const;
    
public slots:
    void setParticleNumber(int particle_number);
    void update(ParticleBody** particle_bodies_list);
protected:
    void paintEvent(QPaintEvent* event);
private:
    int particle_number = 500;
    ParticleImage* particles_list = new ParticleImage [500];
    
    GasSimulator* simulator;
};

class GasSimulatorWidget : public QWidget {
    Q_OBJECT
public:
    GasSimulatorWidget(QWidget* parent = nullptr);
public slots:
    void adjustInitialSpeedLineEdit();
    void adjustInitialSpeedSlider();
    void adjustParticleNumberLineEdit();
    void adjustParticleNumberSlider();
    void adjustRadiusLineEdit();
    void adjustRadiusSlider();
    void gatherResetData();
    void toggleGasSimulatorButtonLabel();
    void updateFrameTimeLabel(double frame_time, double max_frame_time);
signals:
    void sendResetData(int particle_number, double radius, double initial_speed);
private:
    void initUI();
    
    QLabel* frame_time_label;
    
    QLineEdit* initial_speed_line_edit;
    QLineEdit* particle_number_line_edit;
    QLineEdit* radius_line_edit;
    
    QPushButton* toggle_button;
    
    QSlider* initial_speed_slider;
    QSlider* particle_number_slider;
    QSlider* radius_slider;
};

#endif /* GAS_SIMULATOR_INTERFACE_H */

