#ifndef GAS_SIMULATOR_H
#define GAS_SIMULATOR_H

#include <QThread>

// Forward declarations

class ParticleBody;

// Classes

class GasSimulator : public QThread {
    Q_OBJECT
public:
    GasSimulator(QObject* parent = nullptr);
    ~GasSimulator();
    
    void kill();
public slots:
    void reset(int particle_number, double radius, double initial_speed);
    void toggle();
protected:   
    void run() override;
    
    int exec();
signals:
    void frameTimeData(double frame_time);
    void particlesFrameResults(ParticleBody** particle_bodies_list, int particle_number);
    void setRendererParticleNumber(int particle_number);
private:
    int particle_number = 500;
    ParticleBody** particles_list = new ParticleBody* [500];
    
    double initial_speed = 0.5;
    double radius = 5.0;
    
    // Thread safe holder variables
    int new_particle_number = 500;
    
    // Flags
    bool active_flag = true;
    bool reset_flag = true;
    bool running_flag = false;
    
    // Frame time variables
    double acumulated_frame_time = 0.0;
    
    unsigned long frame_count = 1;
};

class ParticleBody {
public:
    ParticleBody(double* position, double mass, double radius, double* velocity = new double[2] {0.0, 0.0});
    ~ParticleBody();
    
    void solveCollision(ParticleBody* particle, double normal_x, double normal_y, double distance);
    void update(double time);
    
    double getInverseMass() const;
    double getMass() const;
    double getRadius() const;
    double* getPosition() const;
    double* getVelocity() const;
    
    //void setPosition(double position_x, double position_y);
    void setVelocity(double velocity_x, double velocity_y);
private:
    const double mass;
    const double radius;
    
    double* position;
    double* velocity;
    
    // Calculation aid variables
    const double effective_height;
    const double effective_width;
    const double inverse_mass;
};

#endif /* GAS_SIMULATOR_H */

