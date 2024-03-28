#include <kafka/KafkaProducer.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>


const std::string KAFKA_BROKERS = "127.0.0.1:19092";
const std::string KAFKA_TOPIC = "test_topic";

int main()
{
    using namespace kafka;
    using namespace kafka::clients::producer;

    const std::string brokers = KAFKA_BROKERS;
    const Topic topic = KAFKA_TOPIC;

    // Prepare the configuration
    const Properties props({{"bootstrap.servers", brokers}});

    // Create a producer
    KafkaProducer producer(props);

    // Prepare delivery callback
    auto deliveryCb = [](const RecordMetadata& metadata, const Error& error) {
        if (!error) {
            std::cout << "Message delivered: " << metadata.toString() << std::endl;
        } else {
            std::cerr << "Message failed to be delivered: " << error.message() << std::endl;
        }
    };

    std::int32_t loop_cnt = 20;    
    for (size_t i = 0; i < loop_cnt; i++)
    {
        std::string line = "hello kafka " + std::to_string(i);
        const ProducerRecord record(topic, Key("K", 1), Value(line.c_str(), line.size()));
        producer.send(record, deliveryCb);
        std::cout << "Message value: " << line << std::endl;
        producer.flush();
    }

    producer.close();
}

