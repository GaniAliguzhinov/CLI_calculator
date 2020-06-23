#include "token.h"

// error() simply disguises throws:
void error(const string& s)
{
	throw runtime_error(s);
}

void error(const string& s, const string& s2)
{
	error(s+s2);
}

void error(const string& s, int i)
{
	ostringstream os;
	os << s <<": " << i;
	error(os.str());
}

void keep_window_open()
{
	cin.clear();
	cout << "Please enter a character to exit\n";
	char ch;
	cin >> ch;
	return;
}

void keep_window_open(string s)
{
	if (s=="") return;
	cin.clear();
	cin.ignore(120,'\n');
	for (;;) {
		cout << "Please enter " << s << " to exit\n";
		string ss;
		while (cin >> ss && ss!=s)
			cout << "Please enter " << s << " to exit\n";
		return;
	}
}


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


Token Token_stream::get()
{
	// if (full) { full=false; return buffer; }
    if (!buffer.empty()) {
        Token t = buffer.back();
        buffer.pop_back();
        return t;
    }
	char ch;
	cin >> noskipws >> ch;

    if (ch == '\n') return Token{PRINT};
    if (isspace(ch)) return get();

    if (ch == 'h' || ch == 'H') {
        char ch2;
        cin >> noskipws >> ch2;
        if (isspace(ch2)) {
            return Token{HELP};
        }
        cin.unget();
    }

	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case PRINT:
	case '=':
    case ',':
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
        cin.unget();
		double val;
		cin >> val;
		return Token(NUMBER,val);
	}
    case declkey_arr[0]:
    {
        if (receive_string(declkey, ch)) {
            return Token{LET};
            break;
        }
    }
    case constkey_arr[0]:
    {
        if (receive_string(constkey, ch)) {
            return Token{CONST};
            break;
        }
    }
    case quitkey_arr[0]:
    {
        if (receive_string(quitkey, ch)){
            return Token{QUIT};
            break;
        }
    }

	default:
		if (isalpha(ch) || ch == '_') {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || ch == '_' || isdigit(ch))) s+=ch;                        // Runtime error 1: changed s=ch to s+=ch
			cin.unget();

            if (s == SQRTKEY) return Token{SQRT};
            if (s == POWKEY) return Token{POW};
			return Token{NAME, s};
		}

		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
    while(!buffer.empty()) {
        char c_comp = buffer.back().kind;
        buffer.pop_back();
        if (c == c_comp) {
            return;
        }
    }

	char ch;
	while (cin >> noskipws >> ch)
		if (ch==c) return;
}
void Token_stream::ignore(char c1, char c2)
{
    while(!buffer.empty()) {
        char c_comp = buffer.back().kind;
        buffer.pop_back();
        if (c1 == c_comp || c2 == c_comp) {
            return;
        }
    }

	char ch;
	while (cin >> noskipws >> ch)
		if (ch==c1 || ch==c2) return;
}


/*
Function that tries to read expected string from input.
If it succeeds, returns true, otherwise false.
If it fails, it also returns all input but first_ch back to cin
Used in Token_stream.
*/
bool receive_string(const string str_expected, char first_ch)
{

    string str_input = "" + to_string(first_ch);
    int i = 0;
    char ch;

    if (str_input[i] != str_expected[i])
        return false;

    while(str_input.size() < str_expected.size() && cin.get(ch)) {
        str_input += ch;
        ++i;
        if (str_input[i] != str_expected[i])
            break;
    }
    if (str_expected == str_input)
        return true;
    else if (str_input.size() > string{"1"}.size())
        for (char c : str_input.substr(1))
            cin.putback(c);
    return false;
}
