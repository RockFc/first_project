#include <cstdlib>  // malloc / free
#include <iostream>
#include <new>

struct Person
{
    int         age;
    std::string name;

    Person(int a, const std::string& n) : age(a), name(n)
    {
        std::cout << "Constructor called\n";
    }

    ~Person()
    {
        std::cout << "Destructor called\n";
    }
};

int main()
{
    // 1️⃣ malloc 只分配原始内存
    void* buffer = std::malloc(sizeof(Person));

    // 2️⃣ placement new 构造对象
    Person* p = new (buffer) Person(25, "Alice");

    std::cout << p->age << " " << p->name << std::endl;

    // 3️⃣ 手动调用析构函数
    p->~Person();

    // 4️⃣ free 原始内存
    std::free(buffer);

    return 0;
}