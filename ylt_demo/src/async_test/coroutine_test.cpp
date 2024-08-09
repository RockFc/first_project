#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "async_simple/coro/Lazy.h"
#include "async_simple/coro/SyncAwait.h"
#include "async_simple/coro/Sleep.h"
#include "executors/SimpleExecutor.h"
#include "async_simple/Promise.h"
#include "async_simple/Try.h"

using namespace async_simple::coro;
using namespace std::chrono_literals;
using namespace async_simple::executors;

Lazy<void> foo() {
    std::cout << "enter foo" << std::endl;
    co_await async_simple::coro::sleep(3s);
    std::cout << "leave foo" << std::endl;
    co_return;
}

Lazy<void> foo2() {
    std::cout << "enter foo2" << std::endl;
    co_await async_simple::coro::sleep(1s);
    std::cout << "leave foo2" << std::endl;
    co_return;

}

Lazy<void> bar() {
    std::cout << "enter bar" << std::endl;
    foo().start([](auto&&){});
    foo2().start([](auto&&){});
    std::cout << "leave bar" << std::endl;
    co_return;
}

Lazy<void> cycle(int sec, const std::string& msg)
{
    while (true)
    {
        co_await async_simple::coro::sleep(sec * 1000ms);
        std::cout << msg << std::endl;
    }
}

void test_Promise()
{
    async_simple::Promise<int> p;
    auto f = p.getFuture()
                .then([](int i) {
                    std::cout << "then1" << std::endl;
                    return i * 2; })
                .then([](async_simple::Try<int> t) { 
                    std::cout << "then2" << std::endl;
                    return t.value() * 2; })
                .then([](int i) { 
                    std::cout << "then3" << std::endl;
                    return i; });
    p.setValue(1);
    // f.wait();
    // std::cout << "f.value(): " << f.value() << std::endl;

    int val = std::move(f).get();
    std::cout << "val: " << val << std::endl;
}

int main() {
    // cycle(1, "1 sec").start([](auto&&){});
    // cycle(3, "   3 sec").start([](auto&&){});

    test_Promise();

    // getchar();
    return 0;
}
