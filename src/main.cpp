#include "dynamic_hedging.cpp"

using namespace std;

// Function to run Part I of the Project

void partI(int iteration) {

    vector<double> stock_prices, option_prices, delta_values, beta_values, h_err_values, daily_err_values, pnl;

    double S0 = 100.0;
    double T = 0.4;
    double mu = 0.05;
    double sigma = 0.24;
    double r = 0.025;
    double N = 100.0;
    double K = 105.0;
    char type = 'c';
    double d = 0.0;

    delta_hedging(S0, T, mu, sigma, r, N, K, type, d, stock_prices, option_prices, delta_values, beta_values, h_err_values, daily_err_values, pnl);
    export_results_partI(stock_prices, option_prices, delta_values, h_err_values, daily_err_values, pnl, "results/part_1/part1_" + to_string(iteration) + ".csv");
}

void partII() {
    char type; 
    string t0, tN, tM; 
    double strike_price;

    getUserInputs(type, t0, tN, tM, strike_price);

    vector<double> rates, sec_prices, op_strike_prices, op_prices;
    vector<string> dates, sec_dates, op_dates, op_exdates, cp_flags; 

    readInterestCSV(t0, tN, rates, dates);
    readSecCSV(t0, tN, sec_prices, sec_dates);
    readOpCSV(t0, tN, tM, type, strike_price, op_dates, op_exdates, cp_flags, op_strike_prices, op_prices);
    
    double left_limit = 0.0001; 
    double right_limit = 1.0;
    double eps = left_limit;
    double d = 0.0;

    vector<double> imp_volatilities, delta_values, beta_values, h_err_values, daily_err_values, pnl;

    realDataDeltaHedging(
        // Option parameters
        t0, tM, type, strike_price, d,
        // Implied volatility parameters
        left_limit, right_limit, eps,  
        // Data loaded from CSV files
        rates, sec_prices, sec_dates, op_prices,
        // Empty output vectors to be filled in-place
        imp_volatilities, delta_values, beta_values, h_err_values, daily_err_values, pnl);

    export_results_partII(dates, sec_prices, op_prices, imp_volatilities, delta_values, daily_err_values, h_err_values, pnl,"results/part_2/part2_result.csv" );
}

int main() {

    srand((unsigned int)time(NULL));

    cout << "==================================================" << endl;
    cout << "Executing Part I of the Project..." << endl;
    cout << "==================================================" << endl;
    for (int i = 1; i <= 1000; i++) partI(i);
    cout << "Part I was succesfully executed! Output CSV files can be foud at results/part_1/" << endl;
    cout << "==================================================" << endl << endl << endl;

    cout << "==================================================" << endl;
    cout << "Input information needed for Part II of the project:" << endl;
    cout << "==================================================" << endl << endl;
    partII();
    cout << endl << "==================================================" << endl;
    cout << "Part II was succesfully executed! The output CSV file can be foud at results/part_2/" << endl;
    cout << "==================================================" << endl;

    return 0;

}

// 2011-01-03
// 2011-10-07
// 2013-01-19