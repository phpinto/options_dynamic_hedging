// All helper functions used in implementing the Dynamic Hedging Strategy

#include "option.h"
#include "black_scholes.h"
#include "stock.cpp"
#include "option.cpp"
#include "black_scholes.cpp"
#include <ql/quantlib.hpp>


// Currency string helper class found at https://stackoverflow.com/questions/22104371/currency-format-using-double-values
class currency_string: public numpunct<char>
{
    char do_decimal_point() const { return '.'; }
    char do_thousands_sep() const { return ','; }
    string do_grouping() const { return "\03"; }
};

// Vector print helper function found at https://www.tutorialspoint.com/how-to-print-out-the-contents-of-a-vector-in-cplusplus
// Double Version
void print_d(std::vector <double> const &a) {
   cout << "The vector elements are : ";

   for(int i=0; i < a.size(); i++)
   cout << a.at(i) << ' ';
}
// String Version
void print_s(std::vector <string> const &a) {
   cout << "The vector elements are : ";

   for(int i=0; i < a.size(); i++)
   cout << a.at(i) << ' ';
}

// Delta Hedging Implementation
void delta_hedging(double S0, double T, double mu, double sigma, double r, double N, double K, char type, double d,
    vector<double> &stock_prices, vector<double> &option_prices, vector<double> &delta_values, vector<double> &beta_values, 
    vector<double> &h_err_values, vector<double> &daily_err_values, vector<double> &pnl) {

    double delta_t = (T / N);	
	double b_delta_t = 1.0 / 252.0;

    BlackScholes curr_option;
    Stock curr_stock;	
    double curr_beta, curr_h_err;

    for (int t = 0; t < N; t++) {

        // Stock Price Series
        if (t == 0) stock_prices.push_back(S0);
        else {
            curr_stock = Stock(stock_prices[t - 1], mu, delta_t, sigma);
            stock_prices.push_back(curr_stock.getPrice());
        }

        // Option Price Series
        curr_option = BlackScholes(type, K, stock_prices[t], r, (T - (t * delta_t)), sigma, d);
        option_prices.push_back(curr_option.getPrice());

        // Option Deltas
        delta_values.push_back(curr_option.getDelta());

        // Calculating Beta Values
        if (t == 0) curr_beta = option_prices[t] - (delta_values[t] * stock_prices[t]);
		else {
            curr_beta = (delta_values[t - 1] * stock_prices[t]);
            curr_beta += (beta_values[t - 1] * exp(r * b_delta_t));
            curr_beta -= (delta_values[t] * stock_prices[t]);	
        } 
		beta_values.push_back(curr_beta);	

        // Calculating Cumulative Hedge Error
		if (t == 0) curr_h_err = 0.0;
		else {
			curr_h_err = (delta_values[t - 1] * stock_prices[t]);
            curr_h_err += (beta_values[t - 1] * exp(r * b_delta_t)) - option_prices[t];
		}		
		h_err_values.push_back(curr_h_err);	
		
        // Calculating Daily Hedge Error
		if (t == 0) daily_err_values.push_back(h_err_values[t]);
		else daily_err_values.push_back(h_err_values[t] - h_err_values[t - 1]);

        // Calculating PNL (without hedge)
		pnl.push_back(option_prices[0] - option_prices[t]);	
    }
	
}

// Function to export results to a csv file
void export_results_partI(vector<double> &stock_prices, vector<double> &option_prices, vector<double> &delta_values, 
    vector<double> &h_err_values, vector<double> &daily_err_values, vector<double> &pnl, string file_name){

    ofstream fh(file_name);	
    fh << "Stock Price" << "," << "Option Price" << "," << "Delta" << "," << "Hedging Error" 
       << "," << "PNL" << "," << "PNL (with hedge)" << endl;

    for (int t = 0; t < stock_prices.size(); t++) {
        fh << stock_prices[t] << "," << option_prices[t] << "," << delta_values[t] << "," 
           << daily_err_values[t]<< "," << pnl[t] << "," << h_err_values[t] << endl;
    }
}

// Calculating the Implied Volatility using the Bisection Algorithm
double implied_volatility(char type, double option_price, double left_limit, double right_limit, double eps, 
                          double K, double S, double r, double T, double d) {
	
	double curr_sigma = (left_limit + right_limit) / 2;
	double curr_option_price = BlackScholes(type, K, S,r, T, curr_sigma, d).getPrice();

    int iterations = 0;
	while ((fabs(curr_option_price - option_price) > eps) && iterations++ < 1000) {
		if (curr_option_price < option_price) left_limit = curr_sigma;
        else if (curr_option_price > option_price) right_limit = curr_sigma;

		curr_sigma = (left_limit + right_limit) / 2;
		curr_option_price = BlackScholes(type, K, S,r, T, curr_sigma, d).getPrice();
	} 

	return curr_sigma;
}

void getUserInputs(char &type, string &t0, string &tN, string &t_maturity, double &strike_price) {
    struct tm tm;

    // Get the Option Type  
    string type_input;
    cout << "Please select the option type: Call (C) or Put (P): ";
    while(true){
        if (!(cin >> type_input)) {
            cin.clear();
		    cout << "Please insert a valid option type (C for Call and P Put): ";
        }
        else {
            type = tolower(type_input[0]);
            if (type != 'c' && type != 'p') {
		        cin.clear();
		        cout << "Please insert a valid option type (C for Call and P Put): ";
	        }
            else break;
        }
	}

    // Get the Start Date
	cout << "Please select the Start Date (yyyy-mm-dd): ";
	cin >> t0;
    while (!strptime(t0.c_str(), "%Y-%m-%d", &tm)) {
        cout << "Please enter a valid date in the \"yyyy-mm-dd\" format:" << endl;
        cin >> t0;
    }
    cout << endl;

    // Get the End Date
	cout << "Please select the End Date (yyyy-mm-dd): ";
	cin >> tN;
    while (!strptime(tN.c_str(), "%Y-%m-%d", &tm)) {
        cout << "Please enter a valid date in the \"yyyy-mm-dd\" format:" << endl;
        cin >> tN;
    }
    cout << endl;

    // Get the Maturity Date
	cout << "Please select the Maturity Date (yyyy-mm-dd): ";
	cin >> t_maturity;
    while (!strptime(t_maturity.c_str(), "%Y-%m-%d", &tm)) {
        cout << "Please enter a valid date in the \"yyyy-mm-dd\" format:" << endl;
        cin >> t_maturity;
    }
    cout << endl;

    // Get the Strike Price
    cout << "Please insert the strike price in US $: ";
        while(!(cin >> strike_price)){
	    	cin.clear();
	    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    	cout << "Please insert a numeric strike price: ";
	    }
}

void readInterestCSV(string t0, string tN, vector<double> &rates, vector<string> &dates) {

    // Reading the interest.csv file
    string input_file = "data/interest.csv";
    ifstream infile(input_file, ifstream::in);

    string curr_date, curr_rate;
    bool inDate = false;

    if (!infile) {
        cerr << "Can't open input file " << input_file << endl;
        exit(1);
    }
    while(infile.good()) {
        getline(infile, curr_date, ',');
        getline(infile, curr_rate, '\n');
        if (curr_date == t0) inDate = true;
        if (inDate) {
            if (isdigit(curr_date[0])) {
                dates.push_back(curr_date);
                rates.push_back(0.01 * stod(curr_rate));
            }
        }
        if (curr_date == tN) inDate = false;
    }
    infile.close();
}

void readSecCSV(string t0, string tN, vector<double> &sec_prices, vector<string> &sec_dates) {

    // Reading the sec_GOOG.csv file
    string input_file = "data/sec_GOOG.csv";
    ifstream infile(input_file, ifstream::in);

    string curr_date, curr_price;
    bool inDate = false;

    if (!infile) {
        cerr << "Can't open input file " << input_file << endl;
        exit(1);
    }
    while(infile.good()) {
        getline(infile, curr_date, ',');
        getline(infile, curr_price, '\n');

        if (curr_date == t0) inDate = true;
        if (inDate) {
            if (isdigit(curr_date[0])) {
                sec_dates.push_back(curr_date);
                sec_prices.push_back(stod(curr_price));
            }
        }
        if (curr_date == tN) inDate = false;
    }
    infile.close(); 
}

void readOpCSV(string t0, string tN, string tM, char type, double K, vector<string> &op_dates, 
               vector<string> &op_exdates, vector<string> &cp_flags, 
               vector<double> &op_strike_prices, vector<double> &op_prices) {

    // Reading the op_GOOG.csv file               
    string input_file = "data/op_GOOG.csv";
    ifstream infile(input_file, ifstream::in);

    string curr_date, curr_end_date, curr_flag, curr_price, curr_bid, curr_offer;
    bool inDate = false;
    bool last_date = false;

    if (!infile) {
        cerr << "Can't open input file " << input_file << endl;
        exit(1);
    }
    while(infile.good()) {
        getline(infile, curr_date, ',');
		getline(infile, curr_end_date, ',');
		getline(infile, curr_flag, ',');
		getline(infile, curr_price, ',');
		getline(infile, curr_bid, ',');
		getline(infile, curr_offer, '\n');

        if (curr_date == t0) inDate = true;
            if (inDate) {

                if (isdigit(curr_date[0]) && (curr_end_date == tM) && (tolower(curr_flag[0]) == type) && (stod(curr_price) == K)) {
                    op_dates.push_back(curr_date);
                    op_exdates.push_back(curr_end_date);
                    cp_flags.push_back(curr_flag);
                    op_strike_prices.push_back(stod(curr_price));
                    op_prices.push_back((stod(curr_bid) + stod(curr_offer)) / 2);
            }
        }
        if ((curr_date == tN) && (!last_date)) last_date = true;
        if ((curr_date != tN) && (last_date)) inDate = false;
    }
    infile.close(); 
}

double getNumOfWorkDays(string t0, string tM) {
    QuantLib::Calendar cal = QuantLib::UnitedStates();
    QuantLib::Date t0_date, tN_date, tM_date;
	
	t0_date = QuantLib::DateParser::parseFormatted(t0, "%Y-%m-%d");
	tM_date = QuantLib::DateParser::parseFormatted(tM, "%Y-%m-%d");

	return cal.businessDaysBetween(t0_date, tM_date, true, true);
}


void realDataDeltaHedging(
    // Option parameters
    string t0, string tM, char type, double K, double d,
    // Implied volatility parameters
    double left_limit, double right_limit, double eps,  
    // Data loaded from CSV files
    vector<double> &rates, vector<double> &sec_prices, vector<string> &op_exdates, vector<double> &op_prices,
    // Empty output vectors to be filled in-place
    vector<double> &imp_volatilities, vector<double> &delta_values, vector<double> &beta_values,
    vector<double> &h_err_values, vector<double> &daily_err_values, vector<double> &pnl) {
    
    double curr_vol, curr_beta, curr_h_err;
    double b_delta_t = 1.0/225.0;
    double T = getNumOfWorkDays(t0, tM) / 225.0;
    BlackScholes curr_option;

    if (op_prices.size() != 0) {
        for (int t = 0; t < rates.size(); t++) {    

            // Computing Implied Volatility
            curr_vol = implied_volatility(type, op_prices[t], left_limit, right_limit, eps, K, sec_prices[t], rates[t], T, d);
            imp_volatilities.push_back(curr_vol);

            // Option Deltas
            curr_option = BlackScholes(type, K, sec_prices[t],rates[t], (T - (t * b_delta_t)), curr_vol, d);
            delta_values.push_back(curr_option.getDelta());

            // Calculating Beta Values
            if (t == 0) curr_beta = op_prices[t] - (delta_values[t] * sec_prices[t]);
            else {
                curr_beta = (delta_values[t - 1] * sec_prices[t]);
                curr_beta += (beta_values[t - 1] * exp(rates[t - 1] * b_delta_t));
                curr_beta -= (delta_values[t] * sec_prices[t]);	
            }
            beta_values.push_back(curr_beta);

            // Calculating Cumulative Hedge Error
            if (t == 0) curr_h_err = 0.0;
            else {
                curr_h_err = (delta_values[t - 1] * sec_prices[t]);
                curr_h_err += (beta_values[t - 1] * exp(rates[t - 1] * b_delta_t)) - op_prices[t];
            }		
            h_err_values.push_back(curr_h_err);	

            // Calculating Daily Hedge Error
            if (t == 0) daily_err_values.push_back(h_err_values[t]);
            else daily_err_values.push_back(h_err_values[t] - h_err_values[t - 1]);

            // Calculating PNL (without hedge)
            pnl.push_back(op_prices[0] - op_prices[t]);	
        }
    }
    else cout << "The inputted Option parameters are not present in the Dataset." << endl;

}

// Function to export results to a csv file
void export_results_partII(vector<string> &dates, vector<double> &stock_prices, vector<double> &option_prices, vector<double> &imp_volatilities, 
    vector<double> &delta_values, vector<double> &daily_err_values, vector<double> &h_err_values, vector<double> &pnl, string file_name){

    ofstream fh(file_name);	
    fh << "Date" << "," << "Stock Price" << "," << "Option Price" << "," << "Implied Volatility" << "," << "Delta" << "," << "Hedging Error" 
       << "," << "PNL" << "," << "PNL (with hedge)" << endl;

    for (int t = 0; t < stock_prices.size(); t++) {
        fh << dates[t] << "," << stock_prices[t] << "," << option_prices[t] << "," << imp_volatilities[t]
           << "," << delta_values[t] << "," << daily_err_values[t]<< "," << pnl[t] << "," << h_err_values[t] << endl;
    }
}

// 2011-01-03
// 2011-10-07
// 2013-01-19