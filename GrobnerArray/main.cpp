#include <regex>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void lineParse(string line);

int main()
{

    string line("-15/3 4 2 3");
    lineParse(line);
    /*string line;
    ifstream myfile("polynomialTest1.txt");
    if (myfile.is_open())
    {
    	while (getline(myfile, line))
    	{
    		cout << line << '\n';
    	}
    	myfile.close();
    }*/

    cin.ignore();
}

//Obtains the fraction from the start of the string
void lineParse(string line)
{
    int inc = 0;//Position of the reader
    string a, b;//The fractions
    char cur;//Current value
    bool frac = false, neg = false;
    while (true)
    {
        cur = line[inc];
        inc++;
        if (cur == ' ')//The coefficient is read
            break;
        if (cur == '/')
            frac = true;
        if ((cur >= '0' && cur <= '9') || cur == '-')
        {
            if(frac)
                b += cur;
            else
                a += cur;
        }
    }
    int numer = stoi(a), denom;
    if (frac)//If the input number was a fraction
        denom = stoi(b);
    else
        denom = 1;
    //Rational rat(numer,denom);
    vector<int> powers;
    string conv;//Holds the value to convert to an int
    while(true)
    {
        if (inc == line.size())
        {
            powers.push_back(stoi(conv));
            break;
        }
        cur = line[inc];
        inc++;
        if (cur == ' ')//The coefficient is read
        {
            powers.push_back(stoi(conv));
            conv = "";//Make it the null string again
        }
        if (cur >= '0' && cur <= '9')
            conv += cur;
    }
    for (int i = 0; i < powers.size(); i++)
    {
        cout << powers[i] << endl;
    }
    //Return Monomial(rat,powers);//Add this to the vector of monomials, to create the polynomial
}
