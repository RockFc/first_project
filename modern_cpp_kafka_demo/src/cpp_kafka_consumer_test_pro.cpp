#include <kafka/KafkaConsumer.h>

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <string>

using namespace kafka;
using namespace kafka::clients::consumer;

const std::string KAFKA_BROKERS = "127.0.0.1:19092";
const std::string KAFKA_TOPIC = "test_topic";
const std::int32_t THREAD_NUM = 4;

std::atomic_bool running = {true};
std::atomic_uint64_t msg_count = {0};

void stopRunning(int sig) {
    if (sig != SIGINT) return;

    if (running) {
        running = false;
    } else {
        // Restore the signal handler, -- to avoid stuck with this handler
        signal(SIGINT, SIG_IGN); // NOLINT
    }
}

void consumer_task(const std::string& task_name)
{
    const std::string brokers = KAFKA_BROKERS; 
    const Topic topic = KAFKA_TOPIC; 

    // Prepare the configuration
    Properties props({{"bootstrap.servers", {brokers}}});
    props.put("group.id", "test-group");

    // Create a consumer instance
    KafkaConsumer consumer(props);

    // Subscribe to topics
    consumer.subscribe({topic});

    bool start_flag = false;
    while (running) {
        if (start_flag == false)
        {
            start_flag = true;
            std::cout << "task " << task_name << " started..." << std::endl;
        }
        
        // Poll messages from Kafka brokers
        auto records = consumer.poll(std::chrono::milliseconds(100));

        for (const auto& record: records) {
            if (!record.error()) {
                msg_count++;
                std::cout << task_name << " got a new message..." << std::endl;
                std::cout << "    Topic    : " << record.topic() << std::endl;
                std::cout << "    Partition: " << record.partition() << std::endl;
                std::cout << "    Offset   : " << record.offset() << std::endl;
                std::cout << "    Timestamp: " << record.timestamp().toString() << std::endl;
                std::cout << "    Headers  : " << toString(record.headers()) << std::endl;
                std::cout << "    Key   [" << record.key().toString() << "]" << std::endl;
                std::cout << "    Value [" << record.value().toString() << "]" << std::endl;
            } else {
                std::cerr << record.toString() << std::endl;
            }
        }
    }

    // No explicit close is needed, RAII will take care of it
    consumer.close();
}

int main()
{
    // Use Ctrl-C to terminate the program
    signal(SIGINT, stopRunning);

    // std::vector<std::thread> threads;

    // for (size_t i = 0; i < THREAD_NUM; i++)
    // {
    //     threads.push_back(std::thread(consumer_task, std::to_string(i)));
    // }

    // for (auto& t: threads)
    // {
    //     t.join();
    // }

    // 单节点部署的kafka,启动多线程同组消费好像并不能提高效率
    consumer_task("1");

    std::cout << "Total message count: " << msg_count << std::endl;

}

