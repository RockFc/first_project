#include "common.h"
#include <future>
#include <sw/redis++/redis++.h>
#include <thread>

using namespace sw::redis;

const std::string PUB_CHANNEL = "pub_channel";

int main()
{

    // 子线程发布
    std::thread pub_thread( []() {
        try
        {
            auto redis = Redis( "tcp://123456@127.0.0.1:6379" );
            for ( size_t i = 0; i < 10; i++ )
            {
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                redis.publish( PUB_CHANNEL, "hello-" + std::to_string( i ) );

                std::cout << "publish_channel: " << PUB_CHANNEL << ", message:"
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
        opts1.socket_timeout = std::chrono::milliseconds( 2000 );

        auto redis1 = Redis( opts1 );

        // sub1's socket_timeout is 2000ms.
        auto sub = redis1.subscriber();

        // Set callback functions.
        sub.on_message( []( std::string channel, std::string msg ) {
            // Process message of MESSAGE type.
            std::cout << "on_message::channel: " << std::move( channel ) << ", msg: " << std::move( msg ) << std::endl << std::endl;
        } );

        sub.on_meta( []( Subscriber::MsgType type, OptionalString channel, long long num ) {
            // Process message of META type.
            if ( channel )
            {
                std::cout << "on_meta::type: " << ( int )type << ", channel: " << *channel << ", num: " << num << std::endl << std::endl;
            }
            else
            {
                std::cout << "on_meta::type: " << ( int )type << ", num: " << num << std::endl << std::endl;
            }
        } );

        // Subscribe to channels and patterns.
        sub.subscribe( PUB_CHANNEL );

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
                pub_thread.join();
                return 1;
            }
        }
    }
    catch ( const Error& e )
    {
        std::cerr << "main_cerr: " << e.what() << std::endl;
        pub_thread.join();
        return 1;
    }

    pub_thread.join();
    return 0;
}
