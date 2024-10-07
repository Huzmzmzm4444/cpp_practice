

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cassert>
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
#include <stdexcept> 
#include <string>

struct Token
{
    char kind;
    double value;
    string name;

    Token(char ch) : kind{ ch }, value{ 0 } {}

    Token(char ch, double val) : kind{ ch }, value{ val } {}

    Token(char ch, string id) : kind{ ch }, name{ id } {}
};

class Token_stream
{
    bool full{ false };
    Token buffer{ '\0' };

public:
    Token_stream() {}

    Token get();
    void putback(Token t);

    void ignore(char);
};

void Token_stream::putback(Token t)
{
    if (full)
        error("putback() into a full buffer");

    buffer = t;
    full = true;
}

constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";
const string quitkey = "quit";

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;
    if (!cin) {
        ch = quit;
    }

    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case print:
    case '=':
    case quit:
        return Token{ ch };

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
        cin.putback(ch);
        double val;
        cin >> val;
        return Token{ number, val };
    }

    default:
        if (isalpha(ch))
        {
            string s;
            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
                s += ch;
            cin.putback(ch);

            if (s == declkey)
                return Token{ let };

            if (s == quitkey)
                return Token{ quit };

            return Token{ name, s };
        }
        error("bad token");
    }
}

void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind)
    {
        full = false;
        return;
    }
    full = false;

    for (char ch; cin >> ch;)
    {
        if (ch == c)
            return;
    }
}

struct Variable
{
    string name;
    double value;

    Variable(string n, double v) : name{ n }, value{ v } {}
};

vector<Variable> var_table;

double get_value(const string& s)
{
    for (const auto& var : var_table)
    {
        if (var.name == s)
            return var.value;
    }

    error("get: undefined name ", s);
}

void set_value(const string& s, double d)
{
    for (auto& var : var_table)
    {
        if (var.name == s)
        {
            var.value = d;
            return;
        }
    }

    error("set: undefined name ", s);
}

bool is_declared(const string& s)
{
    for (const auto& var : var_table)
    {
        if (var.name == s)
            return true;
    }

    return false;
}

double define_name(const string& var, double val)
{
    if (is_declared(var))
        error(var, " declared twice");

    var_table.push_back(Variable{ var, val });

    return val;
}

Token_stream ts;

double expression();

double primary()
{
    Token t = ts.get();
    switch (t.kind)
    {
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')')
            error("'(' expected");
        return d;
    }

    case '-':
        return -primary();
    case '+':
        return +primary();

    case number:
        return t.value;

    case name:
        return get_value(t.name);

    default:
        error("primary expected");
    }
}

double term()
{
    double left = primary();

    while (true)
    {
        Token t = ts.get();

        switch (t.kind)
        {
        case '*':
            left *= primary();
            break;

        case '/':
        {
            double d = primary();
            if (d == 0)
                error("divide by zero");
            left /= d;
            break;
        }

        case '%':
        {
            double d = primary();
            if (d == 0)
                error("'%': divide by zero");
            left = fmod(left, d);
            break;
        }

        default:
            ts.putback(t);
            return left;
        }
    }
}

double expression()
{
    double left = term();

    while (true)
    {
        Token t = ts.get();

        switch (t.kind)
        {
        case '+':
            left += term();
            break;

        case '-':
            left -= term();
            break;

        default:
            ts.putback(t);
            return left;
        }
    }
}

double declaration()
{
    Token t = ts.get();
    if (t.kind != name)
        error("name expected in declaration");

    string var = t.name;
    if (is_declared(var))
        error(var, " declared twice");

    t = ts.get();
    if (t.kind != '=')
        error("'=' missing in declaration of ", var);

    return define_name(var, expression());
}

double statement()
{
    Token t = ts.get();
    switch (t.kind)
    {
    case let:
        return declaration();
    default:
        ts.putback(t);
        return expression();
    }
}

void clean_up_mess() { ts.ignore(print); }

void calculate()
{
    while (true)
        try
    {
        cout << prompt;
        Token t = ts.get();
        while (t.kind == print)
            t = ts.get();
        if (t.kind == quit)
            return;

        ts.putback(t);
        cout << result << statement() << endl;
    }
    catch (runtime_error& e)
    {
        cerr << e.what() << endl;
        clean_up_mess();
    }
}
void test_token() {
    // Test Token creation
    Token t1{ '+' };   // Token for operator '+'
    assert(t1.kind == '+');
    assert(t1.value == 0);
    assert(t1.name.empty());

    Token t2{ '8', 3.14 }; // Token for number with value 3.14
    assert(t2.kind == '8');
    assert(t2.value == 3.14);
    assert(t2.name.empty());

    Token t3{ 'a', "variable_name" }; // Token for variable with name
    assert(t3.kind == 'a');
    assert(t3.value == 0);
    assert(t3.name == "variable_name");
}

void test_token_stream() {
    Token_stream ts;

int main()
try
{
    define_name("pi", 3.141592653589793);
    define_name("e", 2.718281828459045);

    calculate();
    test_token();
    test_token_stream();
    cout << " That s correct !" << endl;

}
catch (exception& e)
{
    cerr << "exception: " << e.what() << endl;
    return 1;
}
catch (...)
{
    cerr << "Oops, unknown exception" << endl;
    return 2;
}


