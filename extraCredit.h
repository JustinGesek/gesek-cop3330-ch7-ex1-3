#ifndef EXTRACREDIT_H
#define EXTRACREDIT_H
#include <string>
/*
 *  UCF COP3330 Fall 2021 Assignment 6 Solution
 *  Copyright 2021 Justin Gesek
 */
 
class Variable{
	public:
	Variable(const std::string&_name, double _value, bool _isAConstant):
		name(_name), 
		value(_value),
		isAConstant(_isAConstant)
		{}
	Variable(): name(), value(0), isAConstant(false) {}
	double getValue() const { return value; }
	bool isConstant() const { return isAConstant; }
	void setValue(double v) { if (!isAConstant) { value = v; } }
	private:
	std::string name;
	double value;
	bool isAConstant;
};
 

#endif