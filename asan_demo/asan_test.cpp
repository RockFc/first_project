#include <iostream>
#include <vector>

void test_mem_leaks()
{
    int* p = new int( 1 );
    std::cout << "test_mem_leaks: " << p << std::endl;
}

void test_mem_leaks2()
{
    for ( size_t i = 0; i < 2; i++ )
    {
        int* p = new int( 1 );
        std::cout << "test_mem_leaks2: " << p << std::endl;
    }
    sleep( 10 );
}

int main()
{
    test_mem_leaks();
    test_mem_leaks2();
    return 0;
}