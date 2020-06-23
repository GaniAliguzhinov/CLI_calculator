/*
 * This is a calculator written as a solution to the 
 * exercises from Programming: Principles and Practice Using C++
 * by Bjarne Stroustrup
 *
 */

#include "token.h"
#include "table.h"

const string prompt = "> ";         // PRINT prompt to indicate the need for input
const string result = "= ";         // PRINT before outputting result of calculations


const char NAME = 'a';
const char NUMBER = '8';

constexpr char declkey_arr[] {"#"};
const string declkey{declkey_arr};
const char LET = 'L';

constexpr char quitkey_arr[] {"exit"};
const string quitkey {quitkey_arr};
const char QUIT = 'Q';


const char HELP = 'H';

constexpr char constkey_arr[] {"const"};
const string constkey {constkey_arr};
const char CONST = 'c';

const char PRINT = ';';

const string SQRTKEY = "sqrt";
const char SQRT = 's';

const string POWKEY = "pow";
const char POW = 'p';

// -----------------------------------------------------------------------------------------------------------------------------
Token_stream ts;        // global input stream
Symbol_table table;

double expression();

// -----------------------------------------------------------------------------------------------------------------------------
// primary() parses user input according to grammar:
// Primary:
//     NUMBER
//     '(' Expression ')'
//     "sqrt("Expression")"
//     "pow(" Expression "," Expression ")"
//     -Primary
//     +Primary
// NUMBER:
//     floating-point-literal
//     Variable
// Variable:
//     alphanumeric string corresponding to a variable stored in a vector
double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{
        double d = expression();
		t = ts.get();
		if (t.kind != ')') error("'(' expected");
        return d;                                                                       // Compile warning 2: unused d; added return d;
	}
	case '-':
		return -primary();
    case '+':
		return primary();
	case NUMBER:
		return t.value;
	case NAME:
		return table.get(t.NAME);
    case SQRT:
    {
        t = ts.get();
        if (t.kind != '(') error("'(' expected in sqrt()");
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("'(' expected");
        if (d < 0) error("sqrt of negative NUMBER");
        return sqrt(d);
    }
    case POW:
    {
        t = ts.get();
        if (t.kind != '(') error("'(' expected in pow()");
        double d = expression();
        t = ts.get();
        if (t.kind != ',') error("',' expected in pow()");
        int i2 = narrow_cast<int>(expression());
        t = ts.get();
        if (t.kind != ')') error("'(' expected");

        return pow(d, i2);
    }
	default:
		error("primary expected");
        return 0.0;
	}
}
// -----------------------------------------------------------------------------------------------------------------------------
// term() parses user input according to grammar:
// Term:
//     Primary
//     Term * Primary
//     Term / Primary
//     Term % Primary
double term()
{
	double left = primary();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '*':
        {
			left *= primary();
			break;
        }
		case '/':
		{
            double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
        case '%':
        {
            double d = primary();
            if (d == 0) error("%:divide by zero");
			left = fmod(left, d);
			break;
        }
		default:
			ts.unget(t);
			return left;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------------
// expression() parses user input according to grammar:
// Expression:
//     Term
//     Expression + Term
//     Expression - Term
double expression()
{
	double left = term();

	while(true) {
		Token t = ts.get();

		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------
// declaration() parses user input according to grammar:
// Declaration:
//      NAME "=" Expression         -> add (NAME, Expression) to the table
double declaration(bool is_const = false)
{
	Token t = ts.get();
	if (t.kind != NAME) error ("NAME expected in declaration");
	string var_NAME = t.NAME;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " , var_NAME);
	double d = expression();
    table.declare(var_NAME, d, is_const);

	return d;
}
void print_help() {
    cout << "Help message:\n";
    cout << "To invoke this message, press " << HELP << "\n";
    cout << "To print result of statement, press " << PRINT << "\n";
    cout << "To declare constant variable, enter " << constkey << " <var_name> = <expression>\n";
    cout << "To declare variable, enter " << declkey << " <var_name> = <expression>\n";
    cout << "To exit, enter " << quitkey << "\n";
    cout << "Available functions: " << SQRTKEY << ", " << POWKEY << "\n";
}
// -----------------------------------------------------------------------------------------------------------------------------
// statement() parses user input according to grammar:
// Statement:
//      "LET" Declaration
//      Expression
double statement()
{
	Token t = ts.get();
	switch(t.kind) {
    case HELP:
        print_help();
        return 0;
	case LET:
		return declaration();
    case CONST:
        return declaration(true);
    case NAME:
    {
        Token t2 = ts.get();
        if (t2.kind == '=') {
            double d = expression();
            table.set(t.NAME, d);
            return d;
        } else {

            ts.unget(t2);
        }
    }
	default:

		ts.unget(t);
		return expression();
	}
}

// -----------------------------------------------------------------------------------------------------------------------------
// Eat all input till PRINT command
void clean_up_mess()
{
	ts.ignore(PRINT, '\n');
}

// -----------------------------------------------------------------------------------------------------------------------------
// calculate() holds the main input loop:
// Input Grammar:
// Calculation:
//      PRINT Calculation       -> eat all PRINT commands
//      Statement               -> PRINT result of statement
//      QUIT                    -> exit
//      Statement Calculation   -> repeat till QUIT command
void calculate()
{
	while(cin) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == PRINT) t=ts.get();
		if (t.kind == QUIT) return;
		ts.unget(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

// -----------------------------------------------------------------------------------------------------------------------------
// main handles nasty exceptions that calculate()
// fails to catch, as well as calling the calculate() loop.
int main(int argc, char ** argv)
	try {
        // Predefined NAMEs:
        table.declare("pi", 3.1415926535, true);


        table.declare("e", 2.7182818284, true);
        table.declare("k", 1000);
        // Input loop:
		calculate();
        // Once user enters QUIT command:
        keep_window_open();
		return 0;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
        keep_window_open("~~");
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
        keep_window_open("~~");
		return 2;
	}
// -----------------------------------------------------------------------------------------------------------------------------
