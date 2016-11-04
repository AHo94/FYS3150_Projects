#include <iostream>
#include <chrono>   // Used to seed random generator based on the time
#include <random>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
//#include <armadillo>

using namespace std;

inline int periodic(int i, int limit, int add){
    // Funcion that ensures periodic boundary conditions
    return (i+limit+add) % (limit);
}

void initialize_system(int L, double **spins){
    // Initializes the system, by setting spin states and calculates the energies and magnetizations.
    std::random_device rd; // obtain a random number from hardware
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Time dependent seed
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distr(0,1); // define the range
    for (int x=0; x < L; x++){
        for (int y=0; y<L; y++){
            int spin = distr(generator);
            if (spin == 0){
                spins[x][y] = -1.0;
            }
            else{
                spins[x][y] = 1.0;
            }
        }
    }
}

double Calculate_E(double **Spin_matrix, int L){
    double Energy = 0;
    /*
    for (int i=0; i<L; i++){
        for (int j=0; j<L; j++){
            Energy -= Spin_matrix[i][j]*(Spin_matrix[periodic(i, L, -1)][j] +
                                         Spin_matrix[i][periodic(j, L, -1)]);
        }
    }
    */

    Energy = 2*(Spin_matrix[0][0]*Spin_matrix[0][1]
            + Spin_matrix[0][0]*Spin_matrix[1][0]
            + Spin_matrix[1][1]*Spin_matrix[0][1]
            + Spin_matrix[1][1]*Spin_matrix[1][0]);

    return Energy;
}

double Calculate_M(double **Spin_matrix, int L){
    double Magnetic_moment = 0;
    for (int i=0; i<L; i++){
        for (int j=0; j<L; j++){
            Magnetic_moment += Spin_matrix[i][j];
        }
    }
    return Magnetic_moment;
}

void Metropolis_method(int L, int MC_cycles, double Temperature, double *Expectation_values){
    // A function that uses the Metropolis method
    //std::random_device rd;
    //std::mt19937_64 generator(rd());
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Time dependent seed
    std::default_random_engine generator(seed);
    //std::uniform_int_distribution<int> distr(0,3); // define the range
    //std::normal_distribution<double> distr(0.0,1.0);
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    double **Spin_matrix;
    Spin_matrix = new double*[L];
    for (int i=0; i<L; i++){
        Spin_matrix[i] = new double[L];
    }
    double EnergySum=0;
    double MSum=0;
    double EnergySquaredSum=0;
    double MSquaredSum=0;
    initialize_system(L, Spin_matrix);

    double currentEnergy = Calculate_E(Spin_matrix, L);
    double currentM = Calculate_M(Spin_matrix, L);

    for (int cycle=1; cycle<=MC_cycles; cycle++){
        // Prøv å flipp L^2 spins før måling av energi igjen
        for (int i=0; i<L; i++){
            for (int j=0; j<L; j++){
                int ix = distr(generator)*L;
                int iy = distr(generator)*L;
                Spin_matrix[ix][iy] *= -1.0;    // Flip a random spin
                double E_t = Calculate_E(Spin_matrix, L);   // Calculate new energy after spin flip
                double dE = E_t - currentEnergy;    // Calculate energy difference
                if (dE <= 0){
                    // Accept new state, use the energy for this state
                    currentEnergy = E_t;
                    currentM = Calculate_M(Spin_matrix, L);
                } else {
                    if (distr(generator) <= exp(-dE/Temperature)){
                        // Accept new state, use new energy
                        currentEnergy = E_t;
                        currentM = Calculate_M(Spin_matrix, L);
                    } else {
                        // Do not accept new state. Flip back spin and use old energy
                        Spin_matrix[ix][iy] *= -1.0;
                    }
                }
            }
        }

        EnergySum += currentEnergy;
        EnergySquaredSum += currentEnergy*currentEnergy;
        MSum += currentM;
        MSquaredSum += currentM*currentM;
    }

    /*
    for (int cycle=1; cycle<MC_cycles; cycle++){
        for (int i=0; i<L; i++){
            for (int j=0; j<L; j++){
                int ix = (distr(generator)*L);
                int iy = (distr(generator)*L);
                int dE = 2*Spin_matrix[ix][iy]*(
                        Spin_matrix[ix][periodic(iy, L, -1)]+
                        Spin_matrix[periodic(ix, L, -1)][iy] +
                        Spin_matrix[ix][periodic(iy, L, 1)]+
                        Spin_matrix[periodic(ix, L, 1)][iy]);
                if (distr(generator) <= exp(-dE/Temperature)){
                    Spin_matrix[ix][iy] *= -1.0;
                    Mag_moment += 2*Spin_matrix[ix][iy];
                    Energy += dE;
                }
            }
        }
    }
    */
    Expectation_values[0] += EnergySum/MC_cycles;
    Expectation_values[1] += EnergySquaredSum/MC_cycles;
    Expectation_values[2] += MSum/MC_cycles;
    Expectation_values[3] += MSquaredSum/MC_cycles;
    Expectation_values[4] += fabs(MSum)/MC_cycles;
}

void write_file(int L, int MC_cycles, double Temperature, double *Expectation_values){
    double E_expectation = Expectation_values[0];
    double E2_expectation = Expectation_values[1];
    double M_expectation = Expectation_values[2];
    double M2_expectation = Expectation_values[3];
    double Mabs_expectation = Expectation_values[4];

    double E_variance = (E2_expectation - E_expectation*E_expectation)/L/L;
    double M_variance = (M2_expectation - M_expectation*M_expectation)/L/L;
    double C_v = E_variance/Temperature/Temperature;
    double chi = M_variance/Temperature/Temperature;
    ofile << setw(15) << setprecision(8) << Temperature;
    ofile << setw(15) << setprecision(8) << E_expectation/L/L;
    ofile << setw(15) << setprecision(8) << C_v;
    ofile << setw(15) << setprecision(8) << Mabs_expectation/L/L;
    ofile << setw(15) << setprecision(8) << chi;



}

int main()
{
    double *Expectation_values;
    Expectation_values = new double[5];
    double T_init = 1.0;     // Temperature = 1.0 kT/J
    int L = 2;  // Number of spins

    int MC_cycles = 500000;
    Metropolis_method(L, MC_cycles, T_init, Expectation_values);
    // Analytical expressions
    double AC_v = 64.0*(4+cosh(8.0/T_init))/(T_init*pow((cosh(8.0/T_init)+3), 2));
    double Achi = 8*(exp(8.0/T_init) + cosh(8.0/T_init) + 3.0/2.0)/(T_init*pow((cosh(8.0/T_init)+3), 2));
    double norm = 1.0/(MC_cycles);
    /*
    cout << "<E> = " << Expectation_values[0] << endl;
    cout << "<E^2> = " << Expectation_values[1] << endl;
    cout << "<M> = " << Expectation_values[2] << endl;
    cout << "<M^2> = " << Expectation_values[3] << endl;
    cout << "|<M>| = " << Expectation_values[4] << endl;
    */
    double C_v = (Expectation_values[1] - Expectation_values[0]*Expectation_values[0])/L/L/T_init;
    double Chi = (Expectation_values[3] - Expectation_values[4]*Expectation_values[4])/L/L/T_init;
    cout << "Number of Monte Carlo cycles = " << MC_cycles << endl;
    cout << "Analytic C_v = " << AC_v << ", Numerical C_v = " << C_v << endl;
    cout << "Analytic Chi = " << Achi << ", Numerical Chi = " << Chi << endl;

    // 4c) Let now L = 20
    L = 20;
    string fileout = "4c.txt";
    ofile.open(fileout);
    double T_final = 2.4;
    Expectation_values = new double[5];
    for (Temperature = T_init; Temperature<=T_final; Temperature=T_init+1.4){
        MC_cycles = 500000;
        Metropolis_method(L, MC_cycles, Temperature, Expectation_values);
        write_file(L, MC_cycles, Temperature, Expectation_values);
    }

    return 0;
}
