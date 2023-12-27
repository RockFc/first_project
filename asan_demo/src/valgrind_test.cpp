#include <iostream>
#include <vector>
#include <unistd.h>
#include <unordered_map>
#include <sys/resource.h>
#include <memory>

using namespace std;

const size_t CNT  = 1000000;

/*
测试验证：大量的hash表创建，将消耗非常多的资源
*/

void test_unordered_map_mem()
{
    unordered_map<int, int> m;
    for (size_t i = 0; i < CNT; ++i)
    {
        m.insert(make_pair(i, i));
    }

    cout << "test_unordered_map_mem date mem:" << CNT * 2 * sizeof(int) / 1024 << " KB" << endl;

    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    std::cout << "test_unordered_map_mem  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
}

void test_unordered_map_mem2()
{
    for (size_t i = 0; i < CNT; ++i)
    {
        auto m = new unordered_map<int, int>();
        m->insert(make_pair(i, i));
    }
    cout << "test_unordered_map_mem2 date mem:" << CNT * 2 * sizeof(int) / 1024 << " KB" << endl;

   struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    std::cout << "test_unordered_map_mem2  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
}

int main()
{
    // test_unordered_map_mem();
    test_unordered_map_mem2();

    return 0;
}