
#include <atomic>
#include <iostream>
#include <unistd.h>

#include <ananas/util/ThreadPool.h>

int getMoney( const std::string& name )
{
    return 10000000;
}

std::string getInfo( int year, const std::string& city )
{
    return "hello " + std::to_string( year ) + " " + city;
}

void test_thread_pool()
{
    ananas::ThreadPool pool;

    pool.Execute( getMoney, "mahuateng" ).Then( []( int money ) { std::cout << "mahuateng has money " << money << std::endl; } );

    pool.Execute( getInfo, 2017, "shanghai" ).Then( []( const std::string& info ) { std::cout << info << std::endl; } );

    // 在某个线程内睡眠1秒
    pool.Execute( ::sleep, 1 );

    pool.Execute( []() { std::cout << "hello thread pool!" << std::endl; } ).Then( []() { std::cout << "hello thread pool again!" << std::endl; } );

    sleep( 5 );

    pool.JoinAll();
}

int main( int ac, char* av[] )
{

    test_thread_pool();
    return 0;
}