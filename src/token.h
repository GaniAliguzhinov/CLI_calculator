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
template<class R, class A> R narrow_cast(const A& a)
{
	R r = R(a);
	if (A(r)!=a) error(string("info loss"));
	return r;
}

template<class T> string to_string(const T& t)
{
	ostringstream os;
	os << t;
	return os.str();
}


void keep_window_open();
void keep_window_open(string s);

struct Token {
	char kind;
	double value;
	string NAME;
    Token() :kind(0) {}                                                         // Is it error? Added default constructor
	Token(char ch) :kind{ch}, value{0} { }
	Token(char ch, double val) :kind{ch}, value{val} { }                           // Error? Changed initialization to C++11 style
    Token(char ch, string n) :kind{ch}, NAME{n}{}                                      // Compile error 1: added constructor
};

class Token_stream {
	vector<Token> buffer;
public:
	Token_stream() : buffer{} { }

	Token get();
	void unget(Token t) { buffer.push_back(t); }

	void ignore(char);
    void ignore(char, char);
};

bool receive_string(string s, char c);
