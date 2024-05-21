#include <iostream>

// 基础函数模板，递归终止条件
void print() {
    std::cout << std::endl;
}

// 变长模板函数
template<typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << " ";
    print(args...); // 递归调用
}

int main() {
    print(1, 2.5, "Hello", 'A', true); // 调用示例
    return 0;
}