#include "common.h"
#include <sw/redis++/redis++.h>

using namespace sw::redis;

int main()
{

    // 子线程发布
    std::async( []() {
        try
        {
            auto redis = Redis( "tcp://123456@127.0.0.1:6379" );
            for ( size_t i = 0; i < 100; i++ )
            {
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                redis.publish( "channel", "hello-" + std::to_string( i ) );
                std::cout << "publish::channel "
                          << "hello-" + std::to_string( i ) << std::endl;
            }
        }
        catch ( const std::exception& e )
        {
            std::cerr << "async_cerr: " << e.what() << '\n';
        }
    } );

    try
    {
        // 主线程订阅
        ConnectionOptions opts1;
        opts1.host           = "127.0.0.1";
        opts1.port           = 6379;
        opts1.password       = "123456";
        opts1.socket_timeout = std::chrono::milliseconds( 100 );

        auto redis1 = Redis( opts1 );

        redis1.set( "key", "val" );
        auto val = redis1.get( "key" );
        if ( val )
        {
            std::cout << *val << std::endl;
        }

        // sub1's socket_timeout is 100ms.
        auto sub = redis1.subscriber();

        // Set callback functions.
        sub.on_message( []( std::string channel, std::string msg ) {
            // Process message of MESSAGE type.
            std::cout << "on_message::channel: " << channel << ", msg: " << msg << std::endl;
        } );

        sub.on_meta( []( Subscriber::MsgType type, OptionalString channel, long long num ) {
            // Process message of META type.
            std::cout << "on_meta::type: " << type << ", channel: " << channel.get_value_or( "" ) << ", num: " << num << std::endl;
        } );

        // Subscribe to channels and patterns.
        sub.subscribe( "channel1" );

        // Consume messages in a loop.
        while ( true )
        {
            try
            {
                sub.consume();
            }
            catch ( const Error& err )
            {
                std::cerr << "main_while_cerr: " << err.what() << std::endl;
            }
        }
    }
    catch ( const Error& e )
    {
        std::cerr << "main_cerr: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
