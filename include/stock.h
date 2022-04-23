#ifndef _STOCK_GUARD
#define _STOCK_GUARD
#include<random>

class Stock {
    protected:
        double price;
    public:
        Stock();
        Stock(double prevPrice, double mu, double deltaTime, double sigma);
        Stock(const Stock &rhs);
        ~Stock();

        double getPrice();
};

#endif