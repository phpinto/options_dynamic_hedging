#include "stock.h"
#include  <iostream>

using namespace std;

// Default Constructor
Stock::Stock(){
    price = 0.0;
}

// Helper function to obtain independent standard Normal random variables using the Box Muller method
double boxMuller(){

    double z = 1.0;
    double x, y;
    
    while (z >= 1.0) {
        x = ((2.0 * rand()) / (double)(RAND_MAX)) - 1;
        y = ((2.0 * rand()) / (double)(RAND_MAX)) - 1;
        z = pow(x,2) + pow(y,2);
    }
    double sample = x * sqrt(-2 * log(z)/z); 
    return sample;
}

// Parametrized Constructor
Stock::Stock(double prev_price, double mu, double delta_t, double sigma){
    double Zt = boxMuller();
    price = prev_price + (mu * prev_price * delta_t) + (sigma * prev_price * sqrt(delta_t) * Zt);
}

// Copy Constructor
Stock::Stock(const Stock &rhs){
    price = rhs.price;   
};

// Destructor
Stock::~Stock(){}; 

// Stock Price Getter
double Stock::getPrice() {
    return price;
};