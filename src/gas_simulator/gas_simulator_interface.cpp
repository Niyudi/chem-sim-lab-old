/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "gas_simulator_interface.h"

#include "../config.h"
#include "gas_simulator.h"

#include <ctime>
#include <math.h>
#include <sstream>
#include <string>

#include <QBrush>
#include <QColor>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSizePolicy>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>

/*
 * Classes
 */

// GasSimulatorRenderer

GasSimulatorRenderer::GasSimulatorRenderer(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(round(GAS_SIMULATOR_WIDTH), round(GAS_SIMULATOR_HEIGHT));
    
    // Initiates gas simulator thread
    this->simulator = new GasSimulator(this);
    QObject::connect(this->simulator, &GasSimulator::particlesFrameResults, this, &GasSimulatorRenderer::update);
    QObject::connect(this->simulator, &GasSimulator::frameTimeData, (GasSimulatorWidget*) parent, &GasSimulatorWidget::updateFrameTimeLabel);
    QObject::connect(this->simulator, &GasSimulator::finished, this->simulator, &QObject::deleteLater);
    this->simulator->start();
}

GasSimulator* GasSimulatorRenderer::getSimulator() const {
    return this->simulator;
}

void GasSimulatorRenderer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    // Colors
    QColor brown(117, 75, 11);
    QColor light_yellow(250, 250, 230);
    //QColor grey(215, 215, 215);
    //QColor blue(5, 38, 252);
    
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
    
    painter.drawRect(0, 0, GAS_SIMULATOR_WIDTH, GAS_SIMULATOR_WIDTH);
    
    brush.setColor(brown);
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(brown);
    pen.setStyle(Qt::SolidLine);
    
    painter.setBrush(brush);
    painter.setPen(pen);
    
    for (auto it = this->particles_list.begin() ; it != this->particles_list.end() ; ++it) {
        painter.drawEllipse(it->position[0], it->position[1], 2 * it->radius, 2 * it->radius);
    }
}

void GasSimulatorRenderer::update(std::vector<ParticleBody>* particle_bodies_list) {
    this->particles_list.clear();
    
    for (auto it = particle_bodies_list->begin() ; it != particle_bodies_list->end() ; ++it) {
        short adjusted_x = round(it->getPosition()[0] - it->getRadius());
        short adjusted_y = round(it->getPosition()[1] - it->getRadius());
        ParticleImage particle = {.position = {adjusted_x, adjusted_y}, .radius = round(it->getRadius())};
        this->particles_list.push_back(particle);
    }
    
    this->repaint();
}

// GasSimulatorWidget

GasSimulatorWidget::GasSimulatorWidget(QWidget* parent) : QWidget(parent) {   
    this->initUI();
}

void GasSimulatorWidget::adjustParticleNumberLineEdit() {
    this->particle_number_line_edit->clear();
    this->particle_number_line_edit->insert(QString::number(this->particle_number_slider->value()));
}

void GasSimulatorWidget::adjustParticleNumberSlider() {   
    this->particle_number_slider->setSliderPosition(this->particle_number_line_edit->text().toInt());
}

void GasSimulatorWidget::initUI() {
    /*
     * Widgets
     */
    
    // Labels
    
    this->frame_time_label = new QLabel("Frame time: 0.000ms/0.000ms (0.0%)", this);
    auto* particle_number_label = new QLabel("Particle number:", this);
    
    // Line edits
    
    this->particle_number_line_edit = new QLineEdit("500", this);
    this->particle_number_line_edit->setFixedWidth(50);
    this->particle_number_line_edit->setValidator(new QIntValidator(1, 1000, this));
    
    // Push buttons
    
    auto* reset_button = new QPushButton("Reset simulation", this);
    this->toggle_button = new QPushButton("Resume simulation", this);
    
    // Renderer
    
    auto* renderer = new GasSimulatorRenderer(this);
    
    // Sliders
    
    this->particle_number_slider = new QSlider(Qt::Horizontal, this);
    this->particle_number_slider->setTracking(true);
    this->particle_number_slider->setRange(1, 1000);
    this->particle_number_slider->setValue(500);
    
    /*
     * Layouts
     */
    
    auto* level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    level0_hbox0->setSpacing(0);
    
    auto* level1_vbox0 = new QVBoxLayout();
    
    auto* level2_hbox0 = new QHBoxLayout();
    auto* level2_hbox1 = new QHBoxLayout();
    
    /*
     * Strucutre
     */
    
    level0_hbox0->addLayout(level1_vbox0, 1);
    level0_hbox0->addWidget(renderer, 0);
    
    level1_vbox0->addStretch(1);
    level1_vbox0->addLayout(level2_hbox0, 0);
    level1_vbox0->addLayout(level2_hbox1, 0);
    level1_vbox0->addWidget(this->frame_time_label, 0);
    level1_vbox0->addStretch(1);
    
    level2_hbox0->addWidget(particle_number_label, 0);
    level2_hbox0->addWidget(this->particle_number_slider, 1);
    level2_hbox0->addWidget(this->particle_number_line_edit, 0);
    
    level2_hbox1->addWidget(this->toggle_button, 0);
    level2_hbox1->addWidget(reset_button, 0);
    
    /*
     * Signals
     */
    
    QObject::connect(this->particle_number_line_edit, &QLineEdit::editingFinished, this, &GasSimulatorWidget::adjustParticleNumberSlider);
    QObject::connect(this->particle_number_slider, &QSlider::valueChanged, this, &GasSimulatorWidget::adjustParticleNumberLineEdit);
    
    QObject::connect(reset_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::reset);
    QObject::connect(this->toggle_button, &QPushButton::clicked, this, &GasSimulatorWidget::toggleGasSimulatorButtonLabel);
    QObject::connect(this->toggle_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::toggle);
}

void GasSimulatorWidget::toggleGasSimulatorButtonLabel() {   
    if (this->toggle_button->text() == "Resume simulation") {
        this->toggle_button->setText("Stop simulation");
    } else {
        this->toggle_button->setText("Resume simulation");
    }
}

void GasSimulatorWidget::updateFrameTimeLabel(double frame_time, double max_frame_time) {
    double time_usage = frame_time / max_frame_time * 100;
    
    std::ostringstream frame_time_stream, max_frame_time_stream, time_usage_stream;
    frame_time_stream.precision(3);
    max_frame_time_stream.precision(3);
    time_usage_stream.precision(1);
    frame_time_stream << std::fixed << frame_time;
    max_frame_time_stream << std::fixed << max_frame_time;
    time_usage_stream << std::fixed << time_usage;
    
    this->frame_time_label->setText(QString::fromStdString("Frame time: " + frame_time_stream.str() + "ms/" + max_frame_time_stream.str() + "ms (" + time_usage_stream.str() + "%)"));
}