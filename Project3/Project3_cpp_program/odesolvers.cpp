#include "odesolvers.h"
#include "solarsystem.h"
#include <iostream>

ODEsolvers::ODEsolvers(double dt):
m_dt(dt)
{

}

void ODEsolvers::Euler_step(SolarSystem &system){
    // Solving Euler's methpd
    system.CalculateAccelerationAndEnergy();

    for (Celestials &body : system.bodies()){
        body.position += body.velocity*m_dt;
        body.velocity += body.acceleration*m_dt;
    }
}

void ODEsolvers::EulerCromer(SolarSystem &system){
    // Solving Euler-Cromer's method
    system.CalculateAccelerationAndEnergy();

    for (Celestials &body : system.bodies()){
        body.velocity += body.acceleration*m_dt;
        body.position += body.velocity*m_dt;
    }
}

void ODEsolvers::Verlet(SolarSystem &system){
    // Solving Verlet's method
    system.CalculateAccelerationAndEnergy();

    double m2_2 = m_dt*m_dt/2.0;

    for (Celestials &body : system.bodies()){
        body.position += body.velocity*m_dt + m2_2*body.acceleration;
        vec3 old_acc = body.acceleration;
        system.CalculateAccelerationAndEnergy();
        body.velocity += (m_dt/2.0)*(body.acceleration + old_acc);
    }
}