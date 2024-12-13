
#include "Logger.h"
#include "ThreadPool.h"
#include "TimeUtil.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

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
    rock::ThreadPool pool;
    pool.SetNumOfThreads(4);

    pool.Execute(getMoney, "mahuateng")
        .Then(
            [](int money)
            {
                std::cout << "mahuateng has money " << money << std::endl;
            });

    pool.Execute(getInfo, 2017, "shanghai")
        .Then(
            [](const std::string& info)
            {
                std::cout << info << std::endl;
            });

    // 在某个线程内睡眠1秒
    pool.Execute(::sleep, 1);

    pool.Execute(
            []()
            {
                show_thread_info();
                std::cout << "hello thread pool!" << std::endl;
            })
        .Then(
            []()
            {
                show_thread_info();
                std::cout << "hello thread pool again!" << std::endl;
            });

    pool.Execute(
            []()
            {
                show_thread_info();
                std::cout << "hello rock!" << std::endl;
            })
        .Then(
            []()
            {
                show_thread_info();
                std::cout << "hello rock again!" << std::endl;
            });

    sleep(5);

    pool.JoinAll();
}

void test_log()
{
    rock::LogManager::Instance().Start();
    // auto log = rock::LogManager::Instance().CreateLog(logDEBUG, logConsole);
    // auto log = rock::LogManager::Instance().CreateLog(logINFO, logConsole);
    // auto log = rock::LogManager::Instance().CreateLog(logALL, logConsole);
    auto log = rock::LogManager::Instance().CreateLog(logALL, logFile, "../log");
    std::cout << "getpid()" << getpid() << std::endl;
    std::cout << "this_thread::get_id=" << std::this_thread::get_id() << std::endl;
    // syscall(SYS_gettid);
    std::cout << "syscall(SYS_gettid)=" << syscall(SYS_gettid) << std::endl;

    DBG(log) << "Hello rock, I am debug log!!!";
    INF(log) << "Hello rock, I am info log!!!";
    WRN(log) << "Hello rock, I am warning log!!!";
    ERR(log) << "Hello rock, I am err log!!!";

    rock::LogManager::Instance().Stop();
}

void test_log_in_thred()
{
    rock::ThreadPool pool;
    pool.SetNumOfThreads(4);

    rock::LogManager::Instance().Start();
    // auto log = rock::LogManager::Instance().CreateLog(logALL, logConsole);
    auto log = rock::LogManager::Instance().CreateLog(logALL, logFile, "../log");

    pool.Execute(
            [log]()
            {
                DBG(log) << "I am debug log!!! I am in child thread.";
                sleep(1);
            })
        .Then(
            [log]()
            {
                INF(log) << "I am info log!!! I and debug log are in the same thread.";
            });

    pool.Execute(
        [log]()
        {
            WRN(log) << "I am warning log!!! I am in child thread.";
        });

    ERR(log) << "I am err log!!! I am in main thread.";

    sleep(3);
    rock::LogManager::Instance().Stop();
}

int main(int ac, char* av[])
{

    test_thread_pool();

    test_log();

    test_log_in_thred();

    return 0;
}