#include "FileLoop.h"
#include "ThreadPool.h"

int main()
{
    try
    {
        rock::ThreadPool tp;    // 创建线程池
        tp.SetNumOfThreads(4);  // 设置线程池中的线程数量为10

        FilePool filePool(100);  // 最大打开100个文件
        float    data[10];
        for (int i = 0; i < 10; ++i)
        {
            data[i] = i * i * 3.14f;  // 模拟数据
        }

        for (int i = 0; i < 1000; ++i)
        {
            tp.Execute(
                [&filePool, &data, i]()
                {
                    try
                    {
                        std::string filename = "file" + std::to_string(i) + ".bin";
                        filePool.WriteToFile(filename, data, sizeof(data));
                    }
                    catch (const std::exception& ex)
                    {
                        std::cerr << "Error: " << ex.what() << std::endl;
                    }
                });
        }

        tp.JoinAll();
        filePool.CloseAllFiles();  // 确保所有文件关闭
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}