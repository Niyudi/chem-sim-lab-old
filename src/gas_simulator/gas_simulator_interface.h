#ifndef GAS_SIMULATOR_INTERFACE_H
#define GAS_SIMULATOR_INTERFACE_H

#include "gas_simulator.h"

#include <vector>

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
    void update(ParticleBody** particle_bodies_list, int particle_number);
protected:
    void paintEvent(QPaintEvent* event);
private:
    std::vector<ParticleImage> particles_list;
    
    GasSimulator* simulator;
};

class GasSimulatorWidget : public QWidget {
    Q_OBJECT
public:
    GasSimulatorWidget(QWidget* parent = nullptr);
public slots:
    void adjustParticleNumberLineEdit();
    void adjustParticleNumberSlider();
    void gatherResetData();
    void toggleGasSimulatorButtonLabel();
    void updateFrameTimeLabel(double frame_time, double max_frame_time);
signals:
    void sendResetData(int particle_number);
private:
    void initUI();
    
    QLabel* frame_time_label;
    QLineEdit* particle_number_line_edit;
    QPushButton* toggle_button;
    QSlider* particle_number_slider;
};

#endif /* GAS_SIMULATOR_INTERFACE_H */

