#include <iostream>

// 基础函数模板，递归终止条件
void print()
{
    std::cout << std::endl;
}

// 变长模板函数
template <typename T, typename... Args>
void print(T first, Args... args)
{
    std::cout << first << " ";
    print(args...);  // 递归调用
}

template <typename... T>
auto sum(T... t)
{
    return (t + ...);
}

int main()
{
    print(1, 2.5, "Hello", 'A', true);
    print(sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    print(sum(1.1, 2.2));
    print(sum(std::string("hello"), std::string(" world")));
    return 0;
}