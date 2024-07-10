#include <boost/asio.hpp>
#include <iostream>

void handler(const boost::system::error_code& ec)
{
    if (!ec)
    {
        std::cout << "Timer expired in handle!" << std::endl;
    }
    else
    {
        std::cout << "Error: " << ec.message() << std::endl;
    }
}

int main()
{
    boost::asio::io_context io_context;

    // 创建一个定时器，设置超时时间为 5 秒
    boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(2));

    // 异步等待定时器超时
    timer.async_wait(&handler);

	boost::asio::deadline_timer timer2(io_context, boost::posix_time::seconds(5));
    timer2.async_wait(
        [](const boost::system::error_code& ec)
        {
            if (!ec)
            {
                std::cout << "Timer expired in lambda!" << std::endl;
            }
            else
            {
                std::cout << "Error: " << ec.message() << std::endl;
            }
        });

    // 运行 IO 上下文，直到所有异步操作完成
    io_context.run();

    return 0;
}
