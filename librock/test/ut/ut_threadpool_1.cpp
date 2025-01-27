#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>
#define private public
#define protected public
#include "Logger.h"
#include "ThreadPool.h"
#include "TimeUtil.h"
#include "common.h"
#undef private
#undef protected

class ThreadPoolTest : public testing::Test
{
public:
    void static SetUpTestCase() {}
    void static TearDownCase() {}

protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

int getMoney(const std::string& name)
{
    return 10000000;
}

std::string getInfo(int year, const std::string& city)
{
    return "hello " + std::to_string(year) + " " + city;
}

TEST_F(ThreadPoolTest, Execute_1)
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

TEST_F(ThreadPoolTest, Execute_2)
{
    rock::ThreadPool pool;
    pool.SetNumOfThreads(4);
    try
    {
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
                    std::cout << "sleep 10 second..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                })
            .Wait(std::chrono::milliseconds(2 * 1000));
    }
    catch (const std::exception& e)
    {
        std::cerr << "catch exception:" << e.what() << '\n';
    }
    pool.JoinAll();
}

TEST_F(ThreadPoolTest, Execute_3)
{
    rock::ThreadPool pool;
    pool.SetNumOfThreads(4);
    size_t count = 0;
    try
    {
        auto fut = pool.Execute(
                           [&count]()
                           {
                               std::cout << "add, count=" << ++count << std::endl;
                           })
                       .Then(
                           [&count]()
                           {
                               std::cout << "add again, count=" << ++count << std::endl;
                           });
        fut.Wait();
        std::cout << "wait count=" << count << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "catch exception:" << e.what() << '\n';
    }
    pool.JoinAll();
}

TEST_F(ThreadPoolTest, Log_1)
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

TEST_F(ThreadPoolTest, Log_2)
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