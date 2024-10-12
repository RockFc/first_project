#include <iostream>

void world(int x, int y)
{
    std::cout << "world(" << x << "," << y << ")" << std::endl;
}

void world(double&& x)
{
    std::cout << "world(&& " << x << ")" << std::endl;
}

void world(const double& x)
{
    std::cout << "world(const& " << x << ")" << std::endl;
}
void hello_args()
{
    auto memfn = [=](auto... args)
    {                    // 让 lambda 接受任意参数
        world(args...);  // 拷贝转发所有参数给 world
    };
    memfn(1, 2);  // 双参数：OK
    memfn(3.14);  // 单参数：OK
}

void hello_args_forward()
{
    auto memfn = [=](auto&&... args)
    {                                                  // 让 lambda 接受万能引用做参数
        world(std::forward<decltype(args)>(args)...);  // 通过 FWD 完美转发给 world，避免引用退化
    };
    int x = 1;
    memfn(x, 2);  // 双参数：OK
    memfn(3.14);  // 单参数：OK
}

int main()
{
    hello_args();
    hello_args_forward();
    return 0;
}

/*
world(1,2)
world(const& 3.14)
world(1,2)
world(&& 3.14)
*/