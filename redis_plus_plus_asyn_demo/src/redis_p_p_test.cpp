#include "common.h"
#include <sw/redis++/async_redis++.h>

using namespace sw::redis;

int main()
{

    try
    {
        ConnectionOptions opts;
        opts.host = "127.0.0.1";
        opts.port = 6379;
        opts.password = "123456";

        ConnectionPoolOptions pool_opts;
        pool_opts.size = 3;

        auto async_redis = AsyncRedis( opts, pool_opts );

        Future< string > ping_res = async_redis.ping();

        // Async interface returning Future object.
        Future< bool > set_res = async_redis.set( "key", "val" );
        cout << "set_res.get(): " << set_res.get() << endl;

        // Async interface with callback.
        async_redis.set( "key", "val", []( Future< bool >&& fut ) {
            try
            {
                auto set_res = fut.get();
                cout << "lamda set_res: " << set_res << endl;
            }
            catch ( const Error& err )
            {
                // handle error
            }
        } );

        Future< Optional< string > > get_res = async_redis.get( "key" );

        async_redis.get( "key", []( Future< OptionalString >&& fut ) {
            try
            {
                auto val = fut.get();
                if ( val )
                    cout << "val: " <<  *val << endl;
                else
                    cout << "not exist" << endl;
            }
            catch ( const Error& err )
            {
                // handle error
            }
        } );

        sleep(2);
    }
    catch ( const Error& e )
    {
        // Error handling.
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
