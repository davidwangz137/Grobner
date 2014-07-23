#include <stdlib.h>
#include <iostream>

using namespace std;

int int_sorter(const void *first_arg, const void *second_arg)
{
	int first = *(int*)first_arg;
	int second = *(int*)second_arg;
	if (first < second)
	{
		return -1;
	}
	else if (first == second)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*template <class A_Type> class calc
{
public:
	A_Type multiply(A_Type x, A_Type y);
	A_Type add(A_Type x, A_Type y);
};
template <class A_Type> A_Type calc<A_Type>::multiply(A_Type x, A_Type y)
{
	return x*y;
}
template <class A_Type> A_Type calc<A_Type>::add(A_Type x, A_Type y)
{
	return x + y;
}*/

int main()
{
	int array[10];
	int i;
	/* fill array */
	for (i = 0; i < 10; ++i)
	{
		array[i] = 10 - i;
	}
	qsort(array, 10, sizeof(int), int_sorter);
	for (i = 0; i < 10; ++i)
	{
		printf("%d\n", array[i]);
	}
	//calc<double> a_calc_class;
	//cout << a_calc_class.multiply(6.0, 7.0);
	cin.ignore();
}