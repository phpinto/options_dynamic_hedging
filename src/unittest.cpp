#include "dynamic_hedging.cpp"

using namespace std;

int main() {

    srand((unsigned int)time(NULL));

    cout << "==================================================" << endl;
    cout << "Testing Building an Option" << endl;
    cout << "==================================================" << endl;
    Option test = Option();
    cout << "==================================================" << endl << endl;
    cout << "Testing Building a Black Scholes Option" << endl;
    cout << "Type: Call, K: 315$, S; 250$, r: 0.025, T: 0.3, sigma: 0.24, dividend: 0" << endl;
    cout << "==================================================" << endl << endl;

    BlackScholes test2 = BlackScholes('C', 315.0, 250.0,0.025, 0.3, 0.24, 0);

    Stock stock = Stock();
    cout << "==================================================" << endl;
    cout << "Testing Building a Stock" << endl;
    cout << "==================================================" << endl << endl;

    cout << "==================================================" << endl;
    cout << "All tests were successful!" << endl;
    cout << "==================================================" << endl << endl;



    return 0;

}

// 2011-01-03
// 2011-10-07
// 2013-01-19