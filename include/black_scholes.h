#ifndef _BLACK_SCHOLES_GUARD
#define _BLACK_SCHOLES_GUARD

#include "option.h"

class BlackScholes: public Option {
    public:
        char type; // Option type ('c' for Call, 'p' for Put)

        BlackScholes();
        BlackScholes(char type, double strike_price, double current_price, double rate, 
                            double time_to_maturity, double volatility, double dividend);
        BlackScholes(const BlackScholes &rhs);
        ~BlackScholes();
        
        double getPrice();
        double getDelta();

        double firstDerivative();
};

#endif