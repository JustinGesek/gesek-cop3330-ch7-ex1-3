#include "extraCredit.h"
#include <cctype>
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <stack>
#include <cmath>
using namespace std;

map<string, Variable> variables;
//scanner
deque<string> parseTokens(string line)
{
	deque < string > tokens;
	size_t i = 0;
	while (i < line.size())
	{
		if (isspace(line[i]))
		{
			i += 1;
			continue;
		}
		if (isalpha(line[i]))
		{
			string word;
			while (i < line.size() && (isalnum(line[i]) || line[i] == '_'))
			{
				word += line[i];
				i += 1;
			}
			tokens.push_back(word);
			continue;
		}
		if (isdigit(line[i]) || line[i] == '.')
		{
			bool sawdot = line[i] == '.';
			string number;
			number += line[i];
			i += 1;
			while (i < line.size() && (isdigit(line[i]) || (!sawdot && line[i] == '.')))
			{
				sawdot |= line[i] == '.';
				number += line[i];
				i += 1;
			}
			tokens.push_back(number);
			continue;
		}
		switch (line[i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '=':
		case '%':
		case '(':
		case ')':
			tokens.push_back(string());
			tokens.back() += line[i];
			i += 1;
			break;
		default:
			cerr << "Unexpected character '" << line[i] << "'!" << endl;
			i += 1;
			break;
		}
	}
	return tokens;
}

bool evaluate(stack<double>& values, string op)
{
	if (values.size() < 2)
	{
		cerr << "Not enough operands for operator " << op << "!" << endl;
		return false;
	}
	if (op == "(")
	{
		return true;
	}
	double r = values.top();
	values.pop();
	double l = values.top();
	values.pop();
	double result =
		op == "+" ? l + r :
		op == "-" ? l - r :
		op == "*" ? l * r :
		op == "/" ? l / r :
		op == "%" ? fmod(l, r) :
		0.0;
	values.push(result);
	return true;
}

double evaluate(deque < string >& tokens)
{
	stack<double> values;
	stack<string> operators;

	while (!tokens.empty())
	{
		string token = tokens.front();
		tokens.pop_front();
		if (isdigit(token[0]) || token[0] == '.')
		{
			values.push(stod(token));
			continue;
		}
		if (isalpha(token[0]) || token[0] == '_')
		{
			if (variables.find(token) == variables.end())
			{
				cerr << "undefined variable " << token << "!" << endl;
				tokens.clear();
				return 0.0;
			}
			values.push(variables[token].getValue());
			continue;
		}
		if (token == "+" || token == "-")
		{
			while (!operators.empty() && (operators.top() == "*" || operators.top() == "/" || operators.top() == "%"))
			{
				if (!evaluate(values, operators.top()))
				{
					tokens.clear();
					return 0.0;
				}
				operators.pop();
			}
			operators.push(token);
			continue;
		}
		if (token == "*" || token == "/" || token == "%")
		{
			operators.push(token);
			continue;
		}
		if (token == "(")
		{
			operators.push(token);
			continue;
		}
		if (token == ")")
		{
			while (!operators.empty() && operators.top() != "(")
			{
				if (!evaluate(values, operators.top()))
				{
					tokens.clear();
					return 0.0;
				}
				operators.pop();
			}
			if (operators.empty())
			{
				cerr << "Mismatched closed parenthesis!" << endl;
				tokens.clear();
				return 0.0;
			}
			operators.pop();
			continue;
		}
	}
	while (!operators.empty())
	{
		if (!evaluate(values, operators.top()))
		{
			tokens.clear();
			return 0.0;
		}
		operators.pop();
	}
	return values.top();
}


int main()
{
	while (!cin.eof())
	{
		string line;
		getline(cin, line);
		deque<string> tokens = parseTokens(line);
		if (tokens.front() == "let")
		{
			tokens.pop_front();
			string v = tokens.front();
			tokens.pop_front();
			if (!isalpha(v[0]))
			{
				cerr << "Expected a variable name to follow 'let'" << endl;
				continue;
			}
			if (tokens.front() != "=")
			{
				cerr << "Expected '=' after 'let <variablename>'" << endl;
				continue;
			}
			tokens.pop_front();
			double value = evaluate(tokens);
			variables[v] = Variable(v, value, false);
			cout << v << " <- " << value << endl;
		}
		else if (tokens.front() == "const")
		{
			tokens.pop_front();
			string v = tokens.front();
			tokens.pop_front();
			if (!isalpha(v[0]))
			{
				cerr << "Expected a variable name to follow 'const'" << endl;
				continue;
			}
			if (tokens.front() != "=")
			{
				cerr << "Expected '=' after 'const <variablename>'" << endl;
				continue;
			}
			tokens.pop_front();
			double value = evaluate(tokens);
			variables[v] = Variable(v, value, true);
			cout << v << " <- " << value << endl;
		}
		else if ((isalpha(tokens[0][0]) || tokens[0][0] == '_') && (tokens.size() > 1 && tokens[1] == "="))
		{
			string v = tokens.front();
			tokens.pop_front();
			tokens.pop_front();
			if (variables.find(v) == variables.end())
			{
				cerr << "Variable " << v << " is not defined!" << endl;
				continue;
			}
			if (variables[v].isConstant())
			{
				cerr << "Variable " << v << " is a constant!" << endl;
				continue;
			}
			double value = evaluate(tokens);
			variables[v].setValue(value);
		}
		else
		{
			double value = evaluate(tokens);
			cout << value << endl;
		}
	}
}
