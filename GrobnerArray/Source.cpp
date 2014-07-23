#include <iostream>
#include <vector>
#include <string>
#include "source.h"

using namespace std;

int size;

int GCD(int a, int b);
void signCheck(int& a, int&b);
int lex(const Monomial a, const Monomial b);
int grl(const Monomial a, const Monomial b);
int grv(const Monomial a, const Monomial b);
void selection(vector<Monomial> &poly, int(*compar)(const Monomial, const Monomial));
void vPrint(vector<Monomial> poly);

string Rational::print(int f){
	if (f == 1)
	{
		if (denom == 1)
			return to_string(-numer);
		return to_string(-numer) + "/" + to_string(denom);
	}
	if (denom == 1)
		return to_string(numer);
	return to_string(numer) + "/" + to_string(denom);
}

Rational::Rational(){
	numer = 1;
	denom = 1;
}

Rational::Rational(int a, int b, bool c){
	numer = a;
	denom = b;
}

Rational::Rational(int a, int b){
	numer = a;
	denom = b;
	if (a < 0 && b < 0)
	{
		numer *= -1;
		denom *= -1;
		a *= -1;
		b *= -1;
	}
	if (a < 0)//Just finding the GCD of the integers and then dividing. Don't have to multiply end result by -1
		a *= -1;
	if (b < 0)
	{
		b *= -1;
		numer *= -1;//Flip the signs so numerator is negative instead of denominator
		denom *= -1;
	}
	while (true)
	{
		if (a == 0)
		{
			a = b;
			break;
		}
		if (b == 0)
			break;
		if (a > b)
			a = a%b;
		else
			b = b%a;
	}
	numer /= a;
	denom /= a;
}

Rational operator+ (const Rational& a, const Rational& b)
{
	int gcd = GCD(a.denom, b.denom);
	int newNumer = a.numer*(b.denom / gcd) + b.numer*(a.denom / gcd);
	int newDenom = a.denom*(b.denom / gcd);
	signCheck(newNumer, newDenom);
	return Rational(newNumer, newDenom, true);//Boolean is extra flag to prevent reduction by GCD again
}

Rational operator- (const Rational& a, const Rational& b)
{
	int gcd = GCD(a.denom, b.denom);
	int newNumer = a.numer*(b.denom / gcd) - b.numer*(a.denom / gcd);
	int newDenom = a.denom*(b.denom / gcd);
	signCheck(newNumer, newDenom);
	return Rational(newNumer, newDenom, true);//Boolean is extra flag to prevent reduction by GCD again
}

Rational operator* (const Rational& a, const Rational& b)
{
	int c = GCD(a.numer, b.denom);
	int d = GCD(b.numer, a.denom);
	int newNumer = (a.numer / c)*(b.numer / d);
	int newDenom = (a.denom / d)*(b.denom / c);
	signCheck(newNumer, newDenom);
	return Rational(newNumer, newDenom, true);//Boolean is extra flag to prevent reduction by GCD again
}

Rational operator/ (const Rational& a, const Rational& b)
{
	int c = GCD(a.numer, b.numer);
	int d = GCD(b.denom, a.denom);
	int newNumer = (a.numer / c)*(b.denom / d);
	int newDenom = (a.denom / d)*(b.numer / c);
	signCheck(newNumer, newDenom);
	return Rational(newNumer, newDenom, true);//Boolean is extra flag to prevent reduction by GCD again
}

Monomial::Monomial(Rational a, int* b){
	coeff = a;
	powers = new int[size];
	for (int i = 0; i < size; i++)
		powers[i] = b[i];
}

string Monomial::print(int f){
	string ret = coeff.print(f) + "[";//Append the coefficient to the front
	for (int i = 0; i < size; i++)
	{
		ret += to_string(powers[i]);
		if (i != size - 1)
			ret += ",";
	}
	return ret + "]";
}

string Monomial::printP(){
	string ret = coeff.print(1);//Append the coefficient to the front
	for (int i = 0; i < size; i++)
	{
		ret += "x" + to_string(i) + "^" + to_string(powers[i]);
	}
	return ret;
}

Polynomial::Polynomial(vector<Monomial> v)
{
	poly = v;
	selection(poly, lex);
}

void Polynomial::sort(string order)
{
	if (order == "lex")//Sort monomials according to lexographical order
		selection(poly, lex);
	if (order == "grl")//Graded lex
		selection(poly, grl);
	if (order == "grv")//Graded reverse lex order
		selection(poly, grv);
}

string Polynomial::print()
{
	string app = "";
	app += poly[0].print(0);
	for (int i = 1; i < poly.size(); i++)
	{
		if (poly[i].coeff.numer < 0)
			app += " - ";
		else
			app += " + ";
		if (poly[i].coeff.numer < 0)//Makes the output prettier by changing + -() into simply -() and inverts - -() into + ()
			app += poly[i].print(1);
		else
			app += poly[i].print(0);
	}
	return app;
}

//A selection sort to sort the monomials
//Used because monomial chains do not get too large
void selection(vector<Monomial> &poly, int(*compar)(const Monomial, const Monomial))
{
	int length = poly.size();
	for (int i = 0; i < length - 1; i++)//Last selection selects itself... so the array is already sorted by then
	{
		for (int j = i + 1; j < length; j++)
		{
			if (compar(poly[i], poly[j]) == -1)
			{
				Monomial swap = poly[j];
				poly[j] = poly[i];
				poly[i] = swap;
			}
		}
	}
}

//Lexographical comparison of monomials
int lex(const Monomial a, const Monomial b)
{
	for (int i = 0; i < size; i++)
	{
		if (a.powers[i] > b.powers[i])
		{
			return 1;
		}
		if (a.powers[i] < b.powers[i])
		{
			return -1;
		}
	}
	return 0;//This never hits unless monomials equal degrees
}

int grl(const Monomial a, const Monomial b)
{
	int sum1 = 0, sum2 = 0;
	for (int i = 0; i < size; i++)
	{
		sum1 += a.powers[i];
		sum2 += b.powers[i];
	}
	if (sum1 > sum2)
		return 1;
	if (sum2 > sum1)
		return -1;
	return lex(a, b);//If total order is the same, then do a lex check on them
}

int grv(const Monomial a, const Monomial b)
{
	int sum1 = 0, sum2 = 0;
	for (int i = 0; i < size; i++)
	{
		sum1 += a.powers[i];
		sum2 += b.powers[i];
	}
	if (sum1 > sum2)
		return 1;
	if (sum2 > sum1)
		return -1;
	for (int i = size-1; i >= 0; i--)//Now do a graded reverse lexographical comparison
	{
		if (a.powers[i] > b.powers[i])
		{
			return -1;
		}
		if (a.powers[i] < b.powers[i])
		{
			return 1;
		}
	}
	return 0;
}

void vPrint(vector<Monomial> poly){
	string app = "";
	app += poly[0].print(0);
	for (int i = 1; i < poly.size(); i++)
	{
		if (poly[i].coeff.numer < 0)
			app += " - ";
		else
			app += " + ";
		if (poly[i].coeff.numer < 0)//Makes the output prettier by changing + -() into simply -() and inverts - -() into + ()
			app += poly[i].print(1);
		else
			app += poly[i].print(0);
	}
	std::cout << app << endl;
}

int GCD(int a, int b)
{
	if (a < 0)
		a *= -1;
	if (b < 0)
		b *= -1;
	while (true)
	{
		if (a == 0)
		{
			return b;
		}
		if (b == 0)
			return a;
		if (a > b)
			a = a%b;
		else
			b = b%a;
	}
}

//Flips signs of fractions numer and denom if appropriate
void signCheck(int& a, int&b)
{
	if (b < 0)
	{
		a *= -1;
		b *= -1;
		return;
	}
}

int main(){
	size = 5;//The number of variables in the field that is worked in. Global variable

	Rational w(-12, 3), x(-26, -5), y(13, -4), z = x / w;

	int arr_1[] = { 1, 2, 1, 5, 0 };
	int* arr1 = arr_1;
	int arr_2[] = { 1, 2, 1, 5, 1 };
	int* arr2 = arr_2;
	int arr_3[] = { 0, 3, 5, 1, 4 };
	int* arr3 = arr_3;
	int arr_4[] = { 2, 0, 2, 0, 0 };
	int* arr4 = arr_4;
	Monomial a(w, arr1);
	Monomial b(x, arr2);
	Monomial c(y, arr3);
	Monomial d(z, arr4);

	vector<Monomial> v;
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(d);
	Polynomial pol(v);
	cout << pol.print() << endl;
	pol.sort("grl");
	cout << pol.print() << endl;
	cin.ignore();
}