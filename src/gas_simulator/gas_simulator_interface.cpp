#include "gas_simulator_interface.h"

#include "../config.h"
#include "gas_simulator.h"

#include <ctime>
#include <math.h>
#include <sstream>
#include <string>

#include <QBrush>
#include <QChar>
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

// Debugger :)
#include <iostream>

/*
 * Classes
 */

// GasSimulatorRenderer

GasSimulatorRenderer::GasSimulatorRenderer(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(round(GAS_SIMULATOR_WIDTH), round(GAS_SIMULATOR_HEIGHT));
    
    // Initiates gas simulator thread
    this->simulator = new GasSimulator(this);
    QObject::connect(this->simulator, &GasSimulator::setRendererParticleNumber, this, &GasSimulatorRenderer::setParticleNumber);
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
    
    for (short i = 0 ; i < this->particle_number ; ++i) {
        painter.drawEllipse(this->particles_list[i].position[0], this->particles_list[i].position[1], 2 * this->particles_list[i].radius, 2 * this->particles_list[i].radius);
    }
}

void GasSimulatorRenderer::setParticleNumber(int particle_number) {
    this->particle_number = particle_number;
    
    delete this->particles_list;
    this->particles_list = new ParticleImage [particle_number];
}

void GasSimulatorRenderer::update(ParticleBody** particle_bodies_list) {
    for (short i = 0 ; i < this->particle_number ; ++i) {
        double radius = particle_bodies_list[i]->getRadius();
        short adjusted_x = round(particle_bodies_list[i]->getPosition()[0] - radius);
        short adjusted_y = round(particle_bodies_list[i]->getPosition()[1] - radius);
        ParticleImage particle = {.position = {adjusted_x, adjusted_y}, .radius = (short) round(radius)};
        this->particles_list[i] = particle;
    }
    
    this->repaint();
}

// GasSimulatorWidget

GasSimulatorWidget::GasSimulatorWidget(QWidget* parent) : QWidget(parent) {   
    this->initUI();
}

void GasSimulatorWidget::adjustInitialSpeedLineEdit() {
    this->initial_speed_line_edit->setText(QString::number(this->initial_speed_slider->value()));
}

void GasSimulatorWidget::adjustInitialSpeedSlider() {   
    this->initial_speed_slider->setValue(this->initial_speed_line_edit->text().toInt());
}

void GasSimulatorWidget::adjustParticleNumberLineEdit() {
    this->particle_number_line_edit->setText(QString::number(this->particle_number_slider->value()));
}

void GasSimulatorWidget::adjustParticleNumberSlider() {   
    this->particle_number_slider->setValue(this->particle_number_line_edit->text().toInt());
}

void GasSimulatorWidget::adjustRadiusLineEdit() {
    this->radius_line_edit->setText(QString::number(this->radius_slider->value()));
}

void GasSimulatorWidget::adjustRadiusSlider() {
    this->radius_slider->setValue(this->radius_line_edit->text().toInt());
}

void GasSimulatorWidget::gatherResetData() {
    this->sendResetData(this->particle_number_slider->value(), this->radius_slider->value() / 100.0, this->initial_speed_slider->value() / 100.0);
}

void GasSimulatorWidget::initUI() {
    /*
     * Widgets
     */
    
    // Labels
    
    this->frame_time_label = new QLabel("Frame time: 0.000ms/0.000ms (0.0%)", this);
    
    auto* initial_speed_label = new QLabel("Initial speed:", this);
    auto* particle_number_label = new QLabel("Particle number:", this);
    auto* radius_label = new QLabel("Radius:", this);
    
    // Line edits
    
    this->initial_speed_line_edit = new QLineEdit("50", this);
    this->initial_speed_line_edit->setValidator(new QIntValidator(1, 100, this));
    this->initial_speed_line_edit->setAlignment(Qt::AlignRight);
    this->initial_speed_line_edit->setFixedWidth(46);
    
    this->particle_number_line_edit = new QLineEdit("500", this);
    this->particle_number_line_edit->setValidator(new QIntValidator(1, 1000, this));
    this->particle_number_line_edit->setAlignment(Qt::AlignRight);
    this->particle_number_line_edit->setFixedWidth(46);
    
    this->radius_line_edit = new QLineEdit("500", this);
    this->radius_line_edit->setValidator(new QIntValidator(100, 1000, this));
    this->radius_line_edit->setAlignment(Qt::AlignRight);
    this->radius_line_edit->setFixedWidth(46);
    
    // Push buttons
    
    auto* reset_button = new QPushButton("Reset simulation", this);
    this->toggle_button = new QPushButton("Resume simulation", this);
    
    // Renderer
    
    auto* renderer = new GasSimulatorRenderer(this);
    
    // Sliders
    
    this->initial_speed_slider = new QSlider(Qt::Horizontal, this);
    this->initial_speed_slider->setTracking(true);
    this->initial_speed_slider->setRange(1, 100);
    this->initial_speed_slider->setValue(50);
    
    this->particle_number_slider = new QSlider(Qt::Horizontal, this);
    this->particle_number_slider->setTracking(true);
    this->particle_number_slider->setRange(1, 1000);
    this->particle_number_slider->setValue(500);
    
    this->radius_slider = new QSlider(Qt::Horizontal, this);
    this->radius_slider->setTracking(true);
    this->radius_slider->setRange(100, 1000);
    this->radius_slider->setValue(500);
    
    /*
     * Layouts
     */
    
    auto* level0_hbox0 = new QHBoxLayout(this);
    level0_hbox0->setContentsMargins(0, 0, 0, 0);
    level0_hbox0->setSpacing(0);
    
    auto* level1_vbox0 = new QVBoxLayout();
    
    auto* level2_hbox0 = new QHBoxLayout();
    auto* level2_hbox1 = new QHBoxLayout();
    auto* level2_hbox2 = new QHBoxLayout();
    auto* level2_hbox3 = new QHBoxLayout();
    
    /*
     * Strucutre
     */
    
    level0_hbox0->addLayout(level1_vbox0, 1);
    level0_hbox0->addWidget(renderer, 0);
    
    level1_vbox0->addStretch(1);
    level1_vbox0->addLayout(level2_hbox0, 0);
    level1_vbox0->addLayout(level2_hbox1, 0);
    level1_vbox0->addLayout(level2_hbox2, 0);
    level1_vbox0->addLayout(level2_hbox3, 0);
    level1_vbox0->addWidget(this->frame_time_label, 0);
    level1_vbox0->addStretch(1);
    
    level2_hbox0->addWidget(particle_number_label, 0);
    level2_hbox0->addWidget(this->particle_number_slider, 1);
    level2_hbox0->addWidget(this->particle_number_line_edit, 0);
    
    level2_hbox1->addWidget(radius_label, 0);
    level2_hbox1->addWidget(this->radius_slider, 1);
    level2_hbox1->addWidget(this->radius_line_edit, 0);
    
    level2_hbox2->addWidget(initial_speed_label, 0);
    level2_hbox2->addWidget(this->initial_speed_slider, 1);
    level2_hbox2->addWidget(this->initial_speed_line_edit, 0);
    
    level2_hbox3->addWidget(this->toggle_button, 0);
    level2_hbox3->addWidget(reset_button, 0);
    
    /*
     * Signals
     */
    
    QObject::connect(this->particle_number_line_edit, &QLineEdit::editingFinished, this, &GasSimulatorWidget::adjustParticleNumberSlider);
    QObject::connect(this->particle_number_slider, &QSlider::valueChanged, this, &GasSimulatorWidget::adjustParticleNumberLineEdit);
    
    QObject::connect(this->radius_line_edit, &QLineEdit::editingFinished, this, &GasSimulatorWidget::adjustRadiusSlider);
    QObject::connect(this->radius_slider, &QSlider::valueChanged, this, &GasSimulatorWidget::adjustRadiusLineEdit);
    
    QObject::connect(this->initial_speed_line_edit, &QLineEdit::editingFinished, this, &GasSimulatorWidget::adjustInitialSpeedSlider);
    QObject::connect(this->initial_speed_slider, &QSlider::valueChanged, this, &GasSimulatorWidget::adjustInitialSpeedLineEdit);
    
    QObject::connect(reset_button, &QPushButton::clicked, this, &GasSimulatorWidget::gatherResetData);
    QObject::connect(this->toggle_button, &QPushButton::clicked, this, &GasSimulatorWidget::toggleGasSimulatorButtonLabel);
    QObject::connect(this->toggle_button, &QPushButton::clicked, renderer->getSimulator(), &GasSimulator::toggle);
    
    QObject::connect(this, &GasSimulatorWidget::sendResetData, renderer->getSimulator(), &GasSimulator::reset);
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