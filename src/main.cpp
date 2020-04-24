//#include <stdlib.h>
//#include <time.h>
//#include <stdarg.h>
//#include <stdbool.h>

#include <fstream>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

#include "MessageQueue.h"
#include "SpinLattice.h"
#include "evaluate.h"
#include "simulate.h"

int main() {
  //==================================================================================
  //   INPUT
  //----------------------------------------------------------------------------------
  int size = 10;      // size of the lattice
  double J = 1.;      // coupling constant
  double B = 0.;      // external magnetic field
  double T = 2.26;    // temperature
  int blockSize = 50; // number of measurements in each block
  int numBlocks =
      100; // number of subsequent blocks which are used for error estimation

  // int num_meas = meas_sweeps / block_size;
  //----------------------------------------------------------------------------------

  std::vector<std::vector<int>> llattice(5, std::vector<int>(5, 0));
  SpinLattice lattice = SpinLattice(size);

  std::ofstream fCluster;
  fCluster.open("../logs/cluster_measurements.txt");

  std::cout << "=======================\n";
  std::cout << " Single cluster method\n";
  std::cout << "=======================\n";
  fCluster << "Single cluster method\n";

  auto queue = MessageQueue<std::vector<double>>();
  std::vector<double> parameters{J, B, T};

  std::thread simulationThread(&simulate, &lattice, parameters, blockSize,
                               &queue);
  std::thread evaluationThread(&evaluate, parameters, &queue);

  simulationThread.join();
  evaluationThread.join();

  fCluster.close();
}

/*
int ising(int nb, int meas_sweeps, int method, FILE *fperror)
{

    double e_mean = 0., e_sq_mean = 0., e_block_mean = 0., mag_mean = 0.,
mag_sq_mean = 0., mag_block_mean = 0.; double heat_cap_mean; double
mag_density_mean; double susc_mean; double energy_density_mean; double
energy_ex; double mag_ex; int mem = 0;
    //--------------------------------------
    int *coor;
    int m;
    int n;
    double deltaE;
    double r;

    double energy, mag;
    double * energy_mem = (double*) malloc(num_meas * sizeof(double));
    double * mag_mem = (double*) malloc(num_meas * sizeof(double));
    int pos = 0;
    int old;
    int nn_coor[4][2];
    int cluster[size*size][2];
    float prob = 1 - exp(-2 * J/T);
    int j, i, top_el, old_spin;

    printf("---------------\n");
    printf("Block size: %i\n", block_size);
    printf("Number of measurements: %i\n", num_meas);
    printf("Number of sweeps: %i\n", block_size*num_meas);
*/
/*
// initialize random spins
for(int j=0; j<size; j++) {
  for (int i = 0; i<size; i++) {
    spin[j][i] = -1 + 2 * ((double)rand()/RAND_MAX > 0.5);
    energy = compute_energy(size, spin, J, B)/(size*size);
    mag = average_lattice(size, size, spin);
  }
}
*/

/*// perform measurements
for (long int t = 0; t < sweeps; t++){
    switch(method){
    case 1:
        ;// Metropolis
        for (int i = 0; i < size; i++){
            m = (long int)rand() * size / RAND_MAX;
            n = (long int)rand() * size / RAND_MAX;
            deltaE = - spin[m][n] * compute_deltaE(size, spin, m, n, J, B);
            if (deltaE <= 0){
                spin[m][n] *= -1.;
                energy += deltaE / (size*size);
                mag += 2.*spin[m][n] / (size*size);
            }
            else{
                r = (double)rand()/RAND_MAX;
                if (((double)rand()/RAND_MAX) <= exp(-deltaE/T)){
                    spin[m][n] *= -1;
                    energy += deltaE / (size*size);
                    mag += 2.*spin[m][n] / (size*size);
                }
            }
        }
        break;
    case 2:
        ;// heat bath
        for (int i = 0; i < size; i++){
            m = (long int)rand() * size / RAND_MAX;
            n = (long int)rand() * size / RAND_MAX;
            old = spin[m][n];
            deltaE = compute_deltaE(size, spin, m, n, J, B);
            r = (double)rand()/RAND_MAX;
            if (r < 1.0/(1.0 + exp(deltaE/T))) {
                spin[m][n] = 1;
            }
            else {
                spin[m][n] = -1;
            }
            if (old != spin[m][n]){
                energy += spin[m][n] * deltaE / (size*size);
                mag += 2.*spin[m][n] / (size*size);
            }
            if (fabs(energy) > 4){
                printf("trouble ");
                energy = compute_energy(size, spin, J, B) / (size*size);
            }
        }
        break;
    case 3:
        ;// single cluster
        m = (long int)rand() * size / RAND_MAX;
        n = (long int)rand() * size / RAND_MAX;
        top_el = 0;
        old_spin = spin[m][n];
        cluster[0][0] = m; cluster[0][1] = n;
        while (top_el >= 0){
            m = cluster[top_el][0]; n = cluster[top_el][1];
            cluster[top_el][0] = 0; cluster[top_el][1] = 0;
            top_el--;
            spin[m][n] = -1*old_spin;
            nn_coor[0][0] = (m + size - 1) % size; nn_coor[0][1] = n;
            nn_coor[1][0] = (m + size + 1) % size; nn_coor[1][1] = n;
            nn_coor[2][0] = m;                     nn_coor[2][1] = (n + size +
1) % size; nn_coor[3][0] = m;                     nn_coor[3][1] = (n + size - 1)
% size; for (int k = 0; k < 4; k++){ j = nn_coor[k][0]; i = nn_coor[k][1]; if
(spin[j][i] == old_spin && (double)rand()/RAND_MAX < prob){ top_el++;
                    cluster[top_el][0] = j; cluster[top_el][1] = i;
                    spin[j][i] = -1*old_spin;
                }
            }
        }
        energy = compute_energy(size, spin, J, B) / (size*size);
        mag = average_lattice(size, size, spin);
        break;
    }

    // record mean values after termalization is over
    if (t > term_sweeps){
        e_block_mean += energy;
        mag_block_mean += fabs(mag);
        e_sq_mean += pow(energy, 2);
        mag_sq_mean += pow(mag, 2);
        fprintf(fperror, "%.20f %.20f\n", energy, mag);
    }

    // save average of every block
    if ((t % block_size == 0) && (t > term_sweeps)){
        energy_mem[pos] = e_block_mean / block_size;
        mag_mem[pos] = mag_block_mean / block_size;
        //printf("%10i: \t Energy = %.4f, \t Magnetization density = %.4f\n", t,
energy_mem[pos], mag_mem[pos]); if (fabs(energy_mem[pos]) > 4) {
            printf("Something went wrong :o\n");
            printf("e_block_mean = %f\n", e_block_mean);
            energy_mem[pos] = 0;
            pos--;
            num_meas = pos;
            break;
        }
        e_block_mean = 0.;
        mag_block_mean = 0.;
        pos++;
    }
}*/

// compute exact solution
// exact_2d(T, B, &energy_ex, &mag_ex);

// print_spin_lattice(size, spin);

// fprint_spin_lattice(size, spin, fp);

/*
    // ERROR ESTIMATION
    // ====================

    // Jackknife method
    double sigma_e = 0, sigma_heat = 0, sigma_mag = 0, sigma_susc = 0;
    double e_JK[num_meas], mag_JK[num_meas];
    double heat_JK = 0, susc_JK = 0;

    e_mean = average(num_meas, energy_mem);
    mag_mean = average(num_meas, mag_mem);

    for (int s = 0; s < num_meas; s++)
    {
        e_JK[s] = 0;
        mag_JK[s] = 0;
        for (int i = 0; i < num_meas; i++)
        {
            if (i == s){
                continue;
            }
            e_JK[s] += energy_mem[i];
            mag_JK[s] += mag_mem[i];
        }
        e_JK[s] /= (num_meas - 1);
        mag_JK[s] /= (num_meas - 1);
        heat_JK += pow((e_JK[s] - e_mean), 2) / num_meas;
        susc_JK += pow((mag_JK[s] - mag_mean), 2) / num_meas;
    }

    heat_JK *= (pow(size, 4)/(T*T));
    susc_JK *= (pow(size, 4)/T);

    for (int s = 0; s < num_meas; s++)
    {
        sigma_e    += (pow((e_JK[s] - e_mean), 2)                           *
(num_meas - 1.)/num_meas); sigma_mag  += (pow((mag_JK[s] - mag_mean), 2) *
(num_meas - 1.)/num_meas); sigma_heat += (pow(pow((e_JK[s] - e_mean),
2)*pow(size, 4)/(T*T) - heat_JK, 2) * (num_meas - 1.)/num_meas); sigma_susc +=
(pow(pow((mag_JK[s] - mag_mean), 2)*pow(size, 4)/T - susc_JK, 2) * (num_meas
- 1.)/num_meas);
    }

    printf("Average energy density:         %.6f +/- %.6f\n", e_mean,
sqrt(sigma_e)); printf("Average heat capacity:          %.6f +/- %.6f\n",
heat_JK, sqrt(sigma_heat)); printf("Average magnetization density:  %.6f +/-
%.6f\n", mag_mean, sqrt(sigma_mag)); printf("Average susceptibility: %.6f +/-
%.6f\n", susc_JK, sqrt(sigma_susc));

    printf("Exact energy:                   %.6f\n", energy_ex);
    printf("Exact magnetization density:    %.6f\n", mag_ex);

    free(energy_mem);
    free(mag_mem);
    return 1;

}*/

// Compute the change in energy caused by transition of a spin from spin down to
// spin up. For computing the change in energy caused by flipping of one spin,
// use -1*spin[m][n] * compute_deltaE()
/*
double compute_deltaE(int size, int spin[size][size], int m, int n, double J,
double B) { int up = (m + size - 1) % size; int down = (m + size + 1) % size;
    int right = (n + size + 1) % size;
    int left = (n + size - 1) % size;
    int NNspins = spin[up][n] + spin[down][n] + spin[m][right] + spin[m][left];
    return  -2. * J * (NNspins + B);
}*/

/*
void print_spin_lattice(int size, int spin[size][size]){
    for (int j = 0; j < size; j++){
        for (int i = 0; i < size; i++) {
            if (spin[j][i] == 1) {
                printf("█");
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void fprint_spin_lattice(int size, int spin[size][size], FILE *fp){
    fp = fopen("spin_lattice.txt", "w" );
    for (int j = 0; j < size; j++){
        for (int i = 0; i < size; i++) {
            if (spin[j][i] == 1) {
                fprintf(fp, "1 ");
            }
            else{
                fprintf(fp, "-1 ");
            }
        }
        fprintf(fp, "\n");
    }
}
*/
/*
double average(int N, double array[N]) {
    double sum = 0;
    for (int i = 0; i < N; i++){
        sum += array[i]/N;
    }

    return sum;
}*/

/*
double average_lattice(int N, int M, int array[N][M]){
    double sum = 0;
    for (int j = 0; j < N; j++){
        for (int i = 0; i < M; i++){
            sum += (double) array[j][i];
        }
    }
    return (sum / (N * M));
}*/

/***********
 * exact_2d *
 ***********/

/*

void exact_2d(double T, double B, double *E, double *M) {
  double x, y;
  double z, Tc, K, K1;
  double pi = 3.14159265358979;

  K = 2.0/T;
  //if(B == 0.0) {
    Tc = -2.0/log(sqrt(2.0) - 1.0); // critical temperature;
    if(T > Tc) {
      *M = 0.0;
    }
    else if(T < Tc) {
      z = exp(-K);
      *M = pow(1.0 + z*z,0.25)*pow(1.0 - 6.0*z*z + pow(z,4),0.125)/sqrt(1.0 -
z*z);
    }
    x = 0.5*pi;
    y = 2.0*sinh(K)/pow(cosh(K),2);
    K1 = ellf(x, y);
    *E = -1.0/tanh(K)*(1. + 2.0/pi*K1*(2.0*pow(tanh(K),2) - 1.0));
  //}

  return;
}
*/

/*******
 * ellf *      Elliptic integral of the 1st kind ( c.f. Numerical Recipes )
 *******/
/*
double ellf(double phi, double ak){
  double ellf;
  double s;

  s=sin(phi);
  ellf=s*rf(pow(cos(phi),2),(1.0-s*ak)*(1.0+s*ak),1.0);

  return ellf;
}

double rf(double x, double y, double z)
{
  double rf,ERRTOL,TINY,BIG,THIRD,C1,C2,C3,C4;
  ERRTOL=0.08; TINY=1.5e-38; BIG=3.0e37; THIRD=1.0/3.0;
  C1=1.0/24.0; C2=0.1; C3=3.0/44.0; C4=1.0/14.0;
  double alamb,ave,delx,dely,delz,e2,e3,sqrtx,sqrty,sqrtz,xt,yt,zt;

  if(min(x,y,z) < 0 || min(x+y,x+z,y+z) < TINY || max(x,y,z) > BIG) {
    printf("invalid arguments in rf\n");
    exit(1);
  }
  xt=x;
  yt=y;
  zt=z;
  do {
    sqrtx=sqrt(xt);
    sqrty=sqrt(yt);
    sqrtz=sqrt(zt);
    alamb=sqrtx*(sqrty+sqrtz)+sqrty*sqrtz;
    xt=0.25*(xt+alamb);
    yt=0.25*(yt+alamb);
    zt=0.25*(zt+alamb);
    ave=THIRD*(xt+yt+zt);
    delx=(ave-xt)/ave;
    dely=(ave-yt)/ave;
    delz=(ave-zt)/ave;
  } while (max(abs(delx),abs(dely),abs(delz)) > ERRTOL);
  e2=delx*dely-pow(delz,2);
  e3=delx*dely*delz;
  rf=(1.0+(C1*e2-C2-C3*e3)*e2+C4*e3)/sqrt(ave);

  return rf;
}

double min(double x, double y, double z)
{
  double m;

  m = (x < y) ? x : y;
  m = (m < z) ? m : z;

  return m;
}

double max(double x, double y, double z)
{
  double m;

  m = (x > y) ? x : y;
  m = (m > z) ? m : z;

  return m;
}
*/