
#include "pde_pricer.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// utils functions

double get_dx(Matrix x, int j)
{
    if (x.cols() < j || j < 0)
    {
        exit(1);
    }
    return x(0, j + 1) - x(0, j);
}

// --------------- //

pde_pricer::pde_pricer(
    double spot,
    double strike,
    double rate,
    double vol,
    double T,
    int lambda,
    int n,
    int m)
{
    this->spot = spot;
    this->strike = strike;
    this->rate = rate;
    this->vol = vol;
    this->T = T;
    this->lambda = lambda;
    this->n = n;
    this->m = m;

    this->time_matrix = Matrix(1, n);
    this->x_matrix = Matrix(1, m);

    this->P = Matrix(m - 1, m - 1);
    this->Q = Matrix(m - 1, m - 1);
    this->V = Matrix(m, 1);
    this->U = Matrix(m, 1);
}

void pde_pricer::print()
{
    cout << "spot " << this->spot << endl;
    cout << "strike " << this->strike << endl;
    cout << "rate " << this->rate << endl;
    cout << "vol " << this->vol << endl;
    cout << "T " << this->T << endl;
    cout << "lambda " << this->lambda << endl;
    cout << "n " << this->n << endl;
    cout << "m " << this->m << endl;

    x_matrix.print();
    time_matrix.print();
    U.print();
    Q.print();
}

void pde_pricer::initPricerCall()
{
    double b = 0.5 * pow(vol, 2);
    double c = -b;
    double theta = 0.5;
    for (int i = 0; i < m; i++)
    {
        float d = (float)i / m;
        double s = spot - lambda * vol + 2 * d * lambda * vol;
        x_matrix(0, i) = log(s / spot) - T * rate;
    }
    for (int j = 0; j < n; j++)
    {
        time_matrix(0, j) = (T / n) * j;
    }
    // U matrix
    for (int k = 0; k < m; k++)
    {
        double F = spot * exp(x_matrix(0, k) + rate * T);
        U(k, 0) = exp(-rate * T) * max(F - strike, 0.0);
        // cout << F - strike;
    }
    // V matrix
    V(0, 0) = 0;
    V(m - 1, 0) = (
        (
            b / (get_dx(x_matrix, m - 2) + get_dx(x_matrix, m - 1))
        ) + (
            2 * theta * c / (get_dx(x_matrix, m - 2)*get_dx(x_matrix, m - 1))
        ) * spot * exp(x_matrix(0, m)) - exp(-rate)
    ) + (
            (
                (1 - theta) * c
            ) / (get_dx(x_matrix, m - 2) * get_dx(x_matrix, m - 1))
    ) * spot * exp(x_matrix(0, m));
    // cout << (2 * theta * c / (get_dx(x_matrix, m - 2)*get_dx(x_matrix, m - 1)))* spot * exp(x_matrix(0, m)) - exp(-rate) << "\n";

    // Q matrix
    for (int j = 0; j < m - 1; j++)
    {
        Q(j, j) = (n / T) - (2 * (1 - theta) * c) / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1));
        if(j < m -2)
        {
            Q(j, j+1) = (1 - theta) * c / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1));
            Q(j+1, j) = (1 - theta) * c / (get_dx(x_matrix, j + 1) * get_dx(x_matrix, j + 2));
        }
    }

    // P matrix
    // TODO
}