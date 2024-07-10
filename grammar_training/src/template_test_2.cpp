#include <iostream>
using namespace std;

template <typename T> class TypeID
{
public:
	static const int ID = -1;
};

template <> class TypeID<int8_t>
{
public:
	static const int ID = 0;
};

template <> class TypeID<int16_t>
{
public:
	static const int ID = 1;
};


template <> class TypeID<int32_t>
{
public:
	static const int ID = 2;
};

template <> class TypeID<int64_t>
{
public:
	static const int ID = 3;
};

int main()
{
	// 当模板实例化时，提供的模板参数能匹配到特定的实例，则完成匹配; 如果不能匹配到任何特化形式的时候，就会去匹配类模板的“原型”形式
	cout << TypeID<int8_t>::ID << endl;
	cout << TypeID<int16_t>::ID << endl;
	cout << TypeID<int32_t>::ID << endl;
	cout << TypeID<int64_t>::ID << endl;
	cout << TypeID<bool>::ID << endl;
	cout << TypeID<double>::ID << endl;
	cout << TypeID<std::string>::ID << endl;
	cout << TypeID<void*>::ID << endl;
	cout << TypeID<int*>::ID << endl;
	
   return 0;
}