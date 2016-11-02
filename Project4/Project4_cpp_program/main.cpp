#include <iostream>
#include <chrono>   // Used to seed random generator based on the time
#include <random>
#include <cmath>

using namespace std;

void Metropolis(int L, int lattice, double *spins, double &E_b, double E_t, double &magnetization){
    // A function that uses the Metropolis method
    std::random_device rd; // obtain a random number from hardware
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Time dependent seed
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<> distr(0,3); // define the range

    int im = lattice-1;
    for (int i=0; i<lattice-1; i++){
        E_b += spins[i]*spins[im];
        im = i;
    }
    for (int i=0; i<lattice; i++){
        magnetization += spins[i];
    }
    int spin_pos = distr(generator);
    spins[spin_pos] *= -1;
    for (int i=0; i<lattice-1; i++){
        E_t += spins[i]*spins[im];
        im = i;
    }
}

void generate_spins(int lattice, double *spins){
    std::random_device rd; // obtain a random number from hardware
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Time dependent seed
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<> distr(0,1); // define the range
    for (int i=0; i<lattice; i++){
        int spin = distr(generator);
        if (spin == 0){
            spins[i] = -1;
        }
        else{
            spins[i] = spin;
        }
    }
}

void initialize_system(int L, int lattice, double *spins, double energies, double magnetization){
    // Initializes the system, by setting spin states and calculates the energies and magnetizations.
    std::random_device rd; // obtain a random number from hardware
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Time dependent seed
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<> distr(0,1); // define the range
    for (int i=0; i<lattice; i++){
        int spin = distr(generator);
        if (spin == 0){
            spins[i] = -1;
        }
        else{
            spins[i] = spin;
        }
    }
    int im = lattice-1;
    for (int i=0; i<lattice-1; i++){
        energies += spins[i]*spins[im];
        im = i;
    }
    for (int i=0; i<lattice; i++){
        magnetization += spins[i];
    }



    /*
    for (int i=0; i<N; i++){
        for (int j=0; j<lattice; j++){
            if (distr(generator) == 0){
                spins[i][j] = -1;
            }
            else{
                spins[i][j] = 1;
            }
        }
    }
    for (int i=0; i<N; i++){
        energies[i] = -2*(spins[i][0]*spins[i][1] + spins[i][0]*spins[i][2]
                + spins[i][1]*spins[i][3] + spins[i][2]*spins[i][3]);
        magnetization[i] = (spins[i][0] + spins[i][1] + spins[i][2] + spins[i][3]);
    }
    */
}

int main()
{
    double *spins, energy_b, energy_t, magnetization;
    double T = 1.0;     // Temperature = 1.0 kT/J
    double beta = 1/T;
    int lattice = 4;
    int L = pow(2, lattice);
    spins = new double[lattice];


    //initialize_system(N, lattice, spins, energies, magnetization);
    double Z, E_mean, E_mean2, M_mean, M_mean2, C_v, chi, totC_v, totChi;
    generate_spins(lattice, spins);
    for (int i=0; i<50; i++){
        Metropolis(L, lattice, spins, energy_b, energy_t, magnetization);
        delta_E = old_energy - energy;
    }
    // Analytical expressions
    double AC_v = 64.0*(4+cosh(8))/(T*pow((cosh(8)+3), 2));
    double Achi = 32.0*(exp(8) + 1)/(T*(cosh(8) + 3));
    /*
    for (int j=0; j<1000; j++){
        energies = new double[N];
        magnetization = new double[N];
        spins = new double*[N];
        for (int i=0; i<N; i++){
            spins[i] = new double[lattice];
        }
        initialize_system(L, lattice, spins, energies, magnetization);
        for (int i=0; i<N; i++){
            Z += exp(-beta*energies[i]);
        }
        for (int i=0; i<N; i++){
            E_mean += (1.0/Z)*(energies[i]*exp(-beta*energies[i]));
            E_mean2 += (1.0/Z)*(pow(energies[i],2)*exp(-beta*energies[i]));
            M_mean += (1.0/Z)*(magnetization[i]*exp(-beta*energies[i]));
            M_mean2 += (1.0/Z)*(pow(magnetization[i], 2)*exp(-beta*energies[i]));
        }

        C_v = (E_mean2 - pow(E_mean, 2))/T;
        chi = (M_mean2 - pow(M_mean, 2))/T;
        totC_v += C_v;
        totChi += chi;
        if (fabs(totC_v/j - AC_v) < 1e-3 && fabs(totChi/j - Achi) < 1e-3){
             cout << "AYY WORKED" << endl;
        }
    }
    cout << "Numerical C_v = " << totC_v/1000.0 << ", Analytical C_v = " << AC_v << endl;
    cout << "Numerical chi = " << totChi/1000.0 << ", Analytical chi = " << Achi << endl;
    */
    /*
    for (int i=0; i<N; i++){
        for (int j=0; j<lattice; j++){
            cout << spins[i][j];
        }
        cout << "\n";
    }
    */
    /*
    for (int i=0; i<N; i++){
        cout << energies[i] << " ";
    }
    */
    return 0;
}
