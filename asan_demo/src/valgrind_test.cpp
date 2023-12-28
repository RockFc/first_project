#include <iostream>
#include <memory>
#include <sys/resource.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

using namespace std;

const size_t CNT = 1000000;

/*
测试验证：大量的hash表创建，将消耗非常多的资源
*/

void test_unordered_map_mem()
{
    unordered_map< int, int > m;
    for ( size_t i = 0; i < CNT; ++i )
    {
        m.insert( make_pair( i, i ) );
    }

    cout << "test_unordered_map_mem date mem:" << CNT * 2 * sizeof( int ) / 1024 << " KB" << endl;

    struct rusage r_usage;
    getrusage( RUSAGE_SELF, &r_usage );
    std::cout << "test_unordered_map_mem  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
}

void test_unordered_map_mem2()
{
    for ( size_t i = 0; i < CNT; ++i )
    {
        auto m = new unordered_map< int, int >();
        m->insert( make_pair( i, i ) );
    }
    cout << "test_unordered_map_mem2 date mem:" << CNT * 2 * sizeof( int ) / 1024 << " KB" << endl;

    struct rusage r_usage;
    getrusage( RUSAGE_SELF, &r_usage );
    std::cout << "test_unordered_map_mem2  Memory usage: " << r_usage.ru_maxrss << " KB" << std::endl;
}

int main()
{
    // test_unordered_map_mem();
    test_unordered_map_mem2();

    return 0;
}

/*
Compilation time: 0.83 sec, absolute running time: 0.34 sec, cpu time: 0.27 sec, memory peak: 51 Mb, absolute service time: 1,31 sec
test_unordered_map_mem date mem:7812 KB
test_unordered_map_mem  Memory usage: 52416 KB

Compilation time: 0.62 sec, absolute running time: 0.31 sec, cpu time: 0.25 sec, memory peak: 125 Mb, absolute service time: 0,96 sec
test_unordered_map_mem2 date mem:7812 KB
test_unordered_map_mem2  Memory usage: 128204 KB

在 C++ 中，`std::unordered_map` 的内存占用通常大于其所存储数据的大小。这种现象可以由几个因素解释：

1. **哈希桶和节点开销**：`std::unordered_map` 使用哈希表来存储元素，其中每个元素被存储在一个节点中。这些节点除了存储键值对数据外，还需要额外的内存来管理哈希表结构，如指向下一个节点的指针等。此外，哈希表本身维护了一系列的“哈希桶”，每个桶可能是一个指针，指向其对应的链表或存储元素的节点。这些结构的内存占用是除了实际数据之外的额外开销。

2. **内存分配和对齐**：在内存分配时，为了提高性能和避免碎片化，分配器可能会分配比实际请求更多的内存。此外，内存对齐也可能导致每个节点占用的内存大于其实际数据大小。

3. **加载因子和重哈希**：`std::unordered_map` 为了保持高效的性能，通常会保持较低的加载因子（元素数量与哈希桶数量的比率）。这意味着为了减少哈希冲突，哈希表可能会保留许多未使用的哈希桶，从而增加内存占用。

4. **元素大小**：如果存储在 `unordered_map` 中的元素本身比较小（如小整数或小型结构），则与这些元素相关的管理开销（如哈希桶和节点结构）在总内存占用中所占的比例可能会更高。

因此，当你观察到 `std::unordered_map` 的总内存消耗远大于其存储的数据大小时，这主要是由于哈希表的内部结构和内存管理开销导致的。为了减少内存占用，可以考虑使用其他更内存高效的数据结构，或者在可能的情况下调整 `unordered_map` 的加载因子和初始桶数量。


*/