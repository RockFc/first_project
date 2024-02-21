#include "common.h"
#include <sw/redis++/redis++.h>

using namespace sw::redis;

int main()
{

    try
    {
        // ***** Redis Cluster *****

        // Create a RedisCluster object, which is movable but NOT copyable.
        auto redis_cluster = RedisCluster( "tcp://127.0.0.1:7001" );

        // RedisCluster has similar interfaces as Redis.
        redis_cluster.set( "key", "value" );
        auto val = redis_cluster.get( "key" );
        if ( val )
        {
            std::cout << *val << std::endl;
        }  // else key doesn't exist.

        // std::vector<std::string> to Redis LIST.
        std::vector< std::string > vec = { "a", "b", "c" };
        redis_cluster.del( "list" );
        redis_cluster.rpush( "list", vec.begin(), vec.end() );

        // std::initializer_list to Redis LIST.
        redis_cluster.rpush( "list", { "a", "b", "c" } );

        // Redis LIST to std::vector<std::string>.
        vec.clear();
        redis_cluster.lrange( "list", 0, -1, std::back_inserter( vec ) );

        show_vec( "list", vec );

        // Keys with hash-tag.
        redis_cluster.set( "key{tag}1", "val1" );
        redis_cluster.set( "key{tag}2", "val2" );
        redis_cluster.set( "key{tag}3", "val3" );

        std::vector< OptionalString > hash_tag_res;
        redis_cluster.mget( { "key{tag}1", "key{tag}2", "key{tag}3" }, std::back_inserter( hash_tag_res ) );
        show_opt_vec("hash_tag_res", hash_tag_res );
    }
    catch ( const Error& e )
    {
        // Error handling.
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
