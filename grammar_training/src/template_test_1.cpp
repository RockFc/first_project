#include <iostream>
using namespace std;

template <typename T> class AddOrPlus
{
	static T Do(T a, T b)
	{
		return T(0);
	}
};

template <> class AddOrPlus<int>
{
public:
	static int Do(int a, int b)
	{
		return a * b;
	}
};

template <> class AddOrPlus<float>
{
public:
	static float Do(float a, float b)
	{
		return a + b;
	}
};

int main()
{
   	cout << "Hello World" << endl;
	cout << AddOrPlus<int>::Do(10, 10) << endl;
	cout << AddOrPlus<float>::Do(10.0f, 10.0f) << endl;
	
   return 0;
}