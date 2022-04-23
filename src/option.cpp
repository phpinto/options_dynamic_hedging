#include "option.h"

using namespace std;

// Initializer Method
void Option::init(){
    K = 0.0;
    S = 0.0;
    r = 0.0;
    T = 0.0;
    sigma = 0.0;
    d = 0.0;
}

// Default Constructor
Option::Option(){
    init();
}

// Parametrized Constructor
Option::Option(double strike_price, double current_price, double rate, double time_to_maturity, double volatility, double dividend){
    K = strike_price;
    S = current_price;
    r = rate;
    T = time_to_maturity;
    sigma = volatility;
    d = dividend;
}

// Copy Constructor
Option::Option(const Option &rhs){
    K = rhs.K;   
    S = rhs.S;
    r = rhs.r;
    T = rhs.T;
    sigma = rhs.sigma;
    d = rhs.d;
};

// Destructor
Option::~Option(){}; 


double Option::GetStrikePrice() {
    return K;
};


double Option::GetCurrentPrice() {
    return S;
};


double Option::GetRate() {
    return r;
};


double Option::GetTimeToMaturity() {
    return T;
};


double Option::GetVolatility() {
    return sigma;
};

double Option::GetDividend() {
    return d;
};