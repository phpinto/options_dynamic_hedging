#include "black_scholes.h"

using namespace std;

// Default Constructor
BlackScholes::BlackScholes() : 
Option() {
    type = ' ';
}

// Parametrized Constructor
BlackScholes::BlackScholes(char rhs_type, double strike_price, double current_price, double rate, double time_to_maturity, double volatility, double dividend) : 
Option(strike_price, current_price, rate, time_to_maturity, volatility, dividend) {
    type = rhs_type;
}

// Copy Constructor
BlackScholes::BlackScholes(const BlackScholes &rhs){
    type = rhs.type;
    K = rhs.K;   
    S = rhs.S;
    r = rhs.r;
    T = rhs.T;
    sigma = rhs.sigma;
    d = rhs.d;
};

// Destructor
BlackScholes::~BlackScholes(){
}

// Helper Function to calculate the Gaussian Cumuliative Distribution Function (CDF)

double GaussianCDF(double d) {
    return 0.5 * erfc(-1 * d * sqrt(0.5));
}

// Black Scholes Pricing Function
double BlackScholes::getPrice(){
    
    // Calculating d1:
    double num1 = log(S/K);
    double num2 = ((0.5 * pow(sigma,2)) + r - d) * T; 
    double den = sigma * sqrt(T);
    double d1 = (num1 + num2) / den;

    // Calculating d2:
    double d2 = d1 - den;

    // Final Black-Scholes Formula:

    double nd1, nd2;
    double nd1_coeff = S * exp(-d * T);
    double nd2_coeff = K * exp(-1 * r * T);
    if (type == 'c') {
        nd1 = GaussianCDF(d1);
        nd2 = GaussianCDF(d2);
        return (nd1_coeff * nd1) - (nd2_coeff * nd2);
    }
    else if (type == 'p') {
        nd1 = GaussianCDF(-d1);
        nd2 = GaussianCDF(-d2);
        return (nd2_coeff * nd2) - (nd1_coeff * nd1);
    }
    else return 0;
}

// Black Scholes Delta Function
double BlackScholes::getDelta(){
    
    // Calculating d1:
    double num1 = log(S/K);
    double num2 = ((0.5 * pow(sigma,2)) + r - d) * T; 
    double den = sigma * sqrt(T);
    double d1 = (num1 + num2) / den;

    // Final Black-Scholes Delta:

    double nd1;

    if (type == 'c') {
        nd1 = GaussianCDF(d1);
        return exp(-d * T) * nd1;
    }
    else if (type == 'p') {
        nd1 = GaussianCDF(-d1);
        return exp(-d * T) * (nd1 - 1);
    }
    else return 0;
}

double BlackScholes::firstDerivative() {

    // Calculating d1:
    double num1 = log(S/K);
    double num2 = ((0.5 * pow(sigma,2)) + r - d) * T; 
    double den = sigma * sqrt(T);
    double d1 = (num1 + num2) / den;

    // Calculating d2:
    double d2 = d1 - den;

    // Calculating the Derivative:

    double deriv = 0.39894244888 * exp(-0.5 * pow(d1,2));
    
    return S * sqrt(T) * deriv * exp(-d * T);
};