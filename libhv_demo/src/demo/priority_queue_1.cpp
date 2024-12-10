#include "hv/hv.h"
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "hv/EventLoopThread.h"

using namespace hv;

struct Item
{
    int                   weight;
    std::string           name;
    std::function<void()> tssk;

    bool operator<(const Item& other) const
    {
        return weight < other.weight;  // 按权重降序排序，即权重越大优先级越高
    }
};

int main()
{
    EventLoopThread     loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    std::priority_queue<Item> pq;

    pq.push({1, "Low Priority",
             []()
             {
                 std::cout << "Low Priority Task" << std::endl;
                 std::this_thread::sleep_for(std::chrono::seconds(1));
             }});
    pq.push({3, "High Priority",
             []()
             {
                 std::cout << "High Priority Task" << std::endl;
                 std::this_thread::sleep_for(std::chrono::seconds(3));
             }});
    pq.push({2, "Medium Priority-1",
             []()
             {
                 std::cout << "Medium Priority-1 Task" << std::endl;
                 std::this_thread::sleep_for(std::chrono::seconds(2));
             }});
    pq.push({2, "Medium Priority-2",
             []()
             {
                 std::cout << "Medium Priority-2 Task" << std::endl;
                 std::this_thread::sleep_for(std::chrono::seconds(2));
             }});

    loop->setTimer(100,
                   [&pq, &loop](TimerID)
                   {
                       if (!pq.empty())
                       {
                           const auto item = pq.top();
                           pq.pop();
                           loop->queueInLoop(
                               [item]()
                               {
                                   //    std::cout << item.weight << " " << item.name << " "
                                   //              << hv_gettid() << std::endl;
                                   item.tssk();
                               });
                       }
                   });

    loop_thread.start();
    loop_thread.join();
    return 0;
}