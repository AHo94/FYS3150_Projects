#ifndef METROPOLIS_QUANTUM_H
#define METROPOLIS_QUANTUM_H
#include "vec3.h"
#include "wavefunctions.h"
#include <chrono>
#include <random>
class Metropolis_Quantum
{
public:
    Metropolis_Quantum();
    double CalculateStepLength(vec3 r1, vec3 r2, double alpha, double omega, double *s);
    double LaplaceAnalytic(vec3 r1, vec3 r2, double alpha, double omega);
    double LaplaceOperator(Wavefunctions &WaveFunc, vec3 r1, vec3 r2, double alpha, double omega);

    void Metropolis_T1(int MC_cycles, Wavefunctions &WaveFunc, double *ExpectationValues, \
                       double alpha, double omega, int ExactEne, int Analytic = 0);

    void Metropolis_T2(int MC_cycles, Wavefunctions &WaveFunc, double *ExpectationValues\
                       , double alpha, double beta, double omega);

    void Metropolis_Virial(int MC_cycles, Wavefunctions &WaveFunc, double *ExpectationValues,
                           double alpha, double beta, double omega, int CoulombInt=1);

private:
};

#endif // METROPOLIS_QUANTUM_H
