#include "counter.h"
int main()
{
    Counter co;
    std::cout << "first call AddOnce:" << co.AddOnce() << std::endl;
    return 0;
}