#include <iostream>
#include <vector>
#include <sanitizer/lsan_interface.h>
#include <unistd.h>

using namespace std;

// 默认情况下，asan只在程序结束时进行内存相关统计工作
void test_mem_leak()
{
    int* p = new int(10);
    cout << "test_mem_leak:" << *p << endl;
}

// 也可以手动调用接口，在程序运行中触发内存检测
void test_mem_leak2()
{
    for (size_t i = 0; i < 2; i++)
    {
        int* p = new int(10);
        cout << "test_mem_leak2:" << *p << endl;    
    }
    // 如果这里调用了__lsan_do_leak_check，则不会等到程序结束，会立刻答应出报错信息；否则会等到程序结束
    // __lsan_do_leak_check();
    sleep(10);
    cout << "test_mem_leak2: after sleep(10)" << endl;
}

void test_nullptr()
{
    int* p = new int(10);
    delete p;
    cout << "test_nullptr:" << *p << endl;
}

void test_overflow()
{
    int* p = new int[2];
    p[2] = 10;
    cout << "test_overflow:" << p[2] << endl;
    delete[] p;
}

int main()
{
    // test_mem_leak();
    // test_nullptr();
    // test_overflow();
    test_mem_leak2();

    return 0;
}