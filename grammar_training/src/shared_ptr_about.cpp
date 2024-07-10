#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

using namespace std;

void shared_ptr_test()
{
    std::shared_ptr< int > ptr( new int( 42 ) );
    std::cout << "Before ptr reset, ptr use count: " << ptr.use_count() << std::endl;  // 1
    auto ptr2 = ptr;
    std::cout << "Before ptr reset, ptr use count: " << ptr.use_count() << std::endl;  // 2

    // 引用计数变为 0， 如果 ptr 所指对象无其他引用，则销毁对象 （此处不会销毁）
    ptr.reset();
    std::cout << "After ptr reset, ptr use count: " << ptr.use_count() << std::endl;                              // 0
    std::cout << "Before ptr2 reset, ptr2 use count: " << ptr2.use_count() << "  *ptr2: " << *ptr2 << std::endl;  // 1 42

    // 引用计数变为 0， 如果 ptr2 所指对象无其他引用，则销毁对象 (此处会销毁)
    ptr2.reset();
    std::cout << "After ptr2 reset, ptr2 use count: " << ptr2.use_count() << std::endl;  // 0
    // std::cout << "*ptr2:" << *ptr2 << std::endl;                                      // error
}

void work_err()
{
    auto ptr = std::make_shared< int >( 42 );

    std::thread t( [ &ptr ]() {
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        std::cout << "Value pointed to by shared_ptr in thread t: " << *ptr << std::endl;
    } );

    // 如果主线程中的销毁操作早于子线程中的使用，那么发生崩溃
    // 因为子现成中传的是引用，共享对象仍然只被 主线程中的 ptr 引用，调用 ptr.rest(), 会将 ptr引用计数变为0，并销毁共享对象
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    ptr.reset();
    std::cout << "reset shared_ptr in main thread " << std::endl;

    t.join();
}

void work_ok()
{
    auto ptr = std::make_shared< int >( 42 );

    std::thread t( [ ptr ]() {
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        std::cout << "Value pointed to by shared_ptr in thread t: " << *ptr << std::endl;
    } );

    // 如果主线程中的销毁操作早于子线程中的使用，也不会发生崩溃
    // 因为子现成中传的是值，共享对象被主线程中的 ptr 和 子线程中的 ptr 引用
    // 主线程调用 ptr.rest(), 会将 主线程 ptr引用计数变为0，但子线程中ptr 仍然引用着共享对象，所以不会销毁共享对象
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    ptr.reset();
    std::cout << "reset shared_ptr in main thread " << std::endl;

    t.join();
}

int main()
{
    shared_ptr_test();
    work_err();
    // work_ok();
    return 0;
}
