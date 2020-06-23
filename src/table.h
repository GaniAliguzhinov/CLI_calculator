#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <regex>
#include <random>
#include <stdexcept>

using namespace std;


// error() simply disguises throws:
void error(const string& s);
void error(const string& s, const string& s2);
void error(const string& s, int i);

// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max


// run-time checked narrowing cast (type conversion). See ???.

// run-time checked narrowing cast (type conversion). See ???.



struct Variable {
	string NAME;
	double value;
    bool is_const;
	Variable(string n, double v) :NAME(n), value(v), is_const{false} { }
    Variable(string n, double v, bool is_const) :NAME(n), value(v), is_const{is_const} { }
};

// vector<Variable> NAMEs;

class Symbol_table {
private:
    vector<Variable> NAMEs;
public:
    Symbol_table () {};
    double get(string s);
    bool is_declared(string s);
    void set(string s, double d);
    double declare(string var, double val, bool is_const=false);
    ~Symbol_table () {NAMEs.clear();};
};
