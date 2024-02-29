
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <cstdlib>

#include <ananas/util/ThreadPool.h>
#include "common.h"


int getMoney(const std::string& name)
{
    return 10000000;
}

std::string getInfo(int year, const std::string& city)
{
    return "hello " + std::to_string(year) + " " + city;
}

void test_thread_pool()
{
    ananas::ThreadPool pool;
    pool.SetNumOfThreads(4);

    pool.Execute(getMoney, "mahuateng").Then([](int money) {
        std::cout << "mahuateng has money " << money << std::endl;
    });

    pool.Execute(getInfo, 2017, "shanghai").Then([](const std::string& info) {
        std::cout << info << std::endl;
    });

    // 在某个线程内睡眠1秒
    pool.Execute(::sleep, 1);

    pool.Execute([]() {
        show_thread_info();
            std::cout << "hello thread pool!" << std::endl;
        })
        .Then([]() {
            show_thread_info();
            std::cout << "hello thread pool again!" << std::endl;
        });

        pool.Execute([]() {
        show_thread_info();
            std::cout << "hello rock!" << std::endl;
        })
        .Then([]() {
            show_thread_info();
            std::cout << "hello rock again!" << std::endl;
        });

    sleep(5);

    pool.JoinAll();
}

int main(int ac, char* av[])
{

    test_thread_pool();
    return 0;
}