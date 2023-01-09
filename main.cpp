/*

    CPP pricer PDE - Msc 203

    Virgile Brian - Quentin Avare - Clément Rivière

*/
#include <iostream>
#include "Matrix.h"
#include "bs_price.h"
#include "pde_pricer.h"

using namespace std;


int main(int argc, char **argv){

    // Matrix test;
    Matrix A = Matrix(3, 4);
    Matrix B = Matrix(1, 3);

    B(0, 0) = 3;
    B(0, 1) = 4;
    B(0, 2) = 2;

    A(0, 0) = 13;
    A(0, 1) = 9;
    A(0, 2) = 7;
    A(0, 3) = 15;
    A(1, 0) = 8;
    A(1, 1) = 7;
    A(1, 2) = 4;
    A(1, 3) = 6;
    A(2, 0) = 6;
    A(2, 1) = 4;
    A(2, 2) = 0;
    A(2, 3) = 3;

    Matrix C = B.dot(A);
    (B * 0.5).print();
    cout << "\n" << endl;
    A.print();
    cout << "\n" << endl;
    C.print();

    // BS pricer test
    double S = 100;  
    double K = 110;
    double q = 0;  
    double r = 0.005;  
    double sigma = 0.3; 
    double T = 1;
    char callPutFlag = 'c';


    std::cout << "Price of the call: " << blackScholes(callPutFlag, S, K,  r, q, sigma,  T) << std::endl;

    pde_pricer pricer = pde_pricer(
        S,
        K,
        r,
        sigma,
        T,
        4,
        6,
        6
    );

    pricer.initPricerCall();

    pricer.print();

    return 0;
};