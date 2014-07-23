#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern int size;

class Rational{
public:
	int numer, denom;
	Rational();
	Rational(int a, int b);
	Rational(int a, int b, bool c);
	string print(int f);
};

class Monomial {
public:
	Rational coeff;
	int * powers;
	Monomial::Monomial(Rational a, int* b);
	string print(int f);
	string printP();
};

class Polynomial {
	vector<Monomial> poly;
public:
	Polynomial(vector<Monomial> v);
	void sort(string order);
	string print();
};

