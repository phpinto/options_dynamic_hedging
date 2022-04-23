#ifndef _OPTION_GUARD
#define _OPTION_GUARD
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <unordered_map>


class Option {
    protected:
        void init();
        double K; // Strike Price
        double S; // Spot Price
        double r; // Risk-free Rate
        double T; // Time to Maturation
        double sigma; // Volatility
        double d; // Dividend 
    public:
        Option();
        Option(double strike_price, double current_price, double rate, double time_to_maturity, double volatility, double dividend);
        Option(const Option &rhs);
        ~Option();

        double GetStrikePrice();
        double GetCurrentPrice();
        double GetRate();
        double GetTimeToMaturity();
        double GetVolatility();
        double GetDividend();
};

#endif