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

Monomial::Monomial(Rational a, vector<int> b){
	coeff = a;
	for (int i = 0; i < size; i++)
		powers.push_back(b[i]);
}

Monomial operator+ (Monomial a, Monomial b)
{
	Rational rat = a.coeff + b.coeff;
	Monomial c(rat, a.powers);
	return c;//Return the new monomial
}

Monomial operator- (Monomial a, Monomial b)
{
	Rational rat = a.coeff - b.coeff;
	Monomial c(rat, a.powers);
	return c;//Return the new monomial
}

Monomial operator* (Monomial a, Monomial b)
{
	Rational rat = a.coeff*b.coeff;
	Monomial c(rat, a.powers);
	for (int i = 0; i < size; i++)
	{
		c.powers[i] += b.powers[i];
	}
	return c;//Return the new monomial
}

Monomial operator/ (Monomial a, Monomial b)
{
	Rational rat = a.coeff/b.coeff;
	Monomial c(rat, a.powers);
	for (int i = 0; i < size; i++)
	{
		c.powers[i] -= b.powers[i];
	}
	return c;//Return the new monomial
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

Polynomial operator* (Monomial a, Polynomial b)
{
	for (int i = 0; i < b.poly.size(); i++)
	{
		b.poly[i] = b.poly[i]*a;//Multiply by the monomial
	}
	return b;//Return the new polynomial
}

Polynomial add (Polynomial a, Polynomial b, int(*compar)(const Monomial, const Monomial))
{
	vector<Monomial> vec;//Holds the polynomial list
	int num1 = a.poly.size();
	int num2 = b.poly.size();
	int inc1 = 0;
	int inc2 = 0;
	while (true)
	{
		if (inc1 == num1 && inc2 == num2)//If all the polynomials are emptied out then break
			break;
		if (inc1 == num1)//Check if at limit first
		{
			vec.push_back(b.poly[inc2]);
			inc2++;
		}
		else
		{
			if (inc2 == num2)//Check if at limit first
			{
				vec.push_back(a.poly[inc1]);
				inc1++;
			}
			else
			{
				switch (compar(a.poly[inc1], b.poly[inc2]))
				{
				case 0:
					vec.push_back(a.poly[inc1] + b.poly[inc2]);
					inc1++, inc2++;
					break;
				case 1:
					vec.push_back(a.poly[inc1]);
					inc1++;
					break;
				case -1:
					vec.push_back(b.poly[inc2]);
					inc2++;
					break;
				}
			}
		}
	}
	return Polynomial(vec);
}

Polynomial mul(Polynomial a, Polynomial b, int(*compar)(const Monomial, const Monomial))
{
	vector<Monomial> vec;
	Polynomial ret(vec);
	for (int i = 0; i < a.poly.size(); i++)
	{
		ret = add(ret, a.poly[i] * b, compar);//Append the new terms from multiplication
	}
	return ret;
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
	if (poly.size() == 0)//If empty polynomial
		return "";
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

	vector<int> arr1 = { 1, 2, 1, 5, 1 };
	vector<int> arr2 = { 1, 2, 1, 5, 0 };
	vector<int> arr3 = { 0, 3, 5, 1, 4 };
	vector<int> arr4 = { 2, 0, 2, 0, 0 };
	Monomial a(w, arr1);
	Monomial b(x, arr2);
	Monomial c(y, arr3);
	Monomial d(z, arr4);

	vector<Monomial> v1, v2;
	v1.push_back(a);
	v1.push_back(b);
	v2.push_back(c);
	v2.push_back(d);
	Polynomial pol1(v1), pol2(v2);
	std::cout << pol1.print() << endl;
	cout << pol2.print() << endl;
	Polynomial pol3 = mul(pol1,pol2,lex);
	cout << pol3.print() << endl;
	cin.ignore();
}