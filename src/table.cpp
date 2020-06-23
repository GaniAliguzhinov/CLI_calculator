#include "table.h"

double Symbol_table::get(string s)
{
	for (size_t i = 0; i<NAMEs.size(); ++i)
		if (NAMEs[i].NAME == s) return NAMEs[i].value;
	error("get: undefined NAME ",s);
    return 0.0;
}

void Symbol_table::set(string s, double d)
{
	for (size_t i = 0; i<NAMEs.size(); ++i)
		if (NAMEs[i].NAME == s) {
            if (NAMEs[i].is_const)
                error("set: constant variable ", NAMEs[i].NAME);
			NAMEs[i].value = d;
			return;
		}
	error("set: undefined NAME ",s);
}
// -----------------------------------------------------------------------------------------------------------------------------
// define_NAME() adds (var, val) to the table of variables called <NAMEs>
double Symbol_table::declare(string var, double val, bool is_const)
{
    if (is_declared(var)) error(var, "declared twice");
    NAMEs.push_back(Variable(var, val, is_const));
    return val;
}
// -----------------------------------------------------------------------------------------------------------------------------
bool Symbol_table::is_declared(string s)
{
	for (size_t i = 0; i<NAMEs.size(); ++i)
		if (NAMEs[i].NAME == s) return true;
	return false;
}
