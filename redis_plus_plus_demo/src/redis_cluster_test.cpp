#include <iostream>
#include <sw/redis++/redis++.h>

using namespace sw::redis;
using namespace std;

auto show_vec = []( const std::string& name, const auto& v ) {
    cout << name + " [ ";
    for ( const auto& e : v )
    {
        cout << e << ", ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_map = []( const std::string& name, const auto& m ) {
    cout << name << " [ ";
    for ( const auto& e : m )
    {
        cout << "(" << e.first << ", " << e.second << "), ";
    }
    cout << "]";
    cout << endl << endl;
};

int main()
{

    try
    {
        // ***** Redis Cluster *****

        // Create a RedisCluster object, which is movable but NOT copyable.
        auto redis_cluster = RedisCluster( "tcp://127.0.0.1:7000" );

        // RedisCluster has similar interfaces as Redis.
        redis_cluster.set( "key", "value" );
        auto val = redis_cluster.get( "key" );
        if ( val )
        {
            std::cout << *val << std::endl;
        }  // else key doesn't exist.

        // Keys with hash-tag.
        redis_cluster.set( "key{tag}1", "val1" );
        redis_cluster.set( "key{tag}2", "val2" );
        redis_cluster.set( "key{tag}3", "val3" );

        std::vector< OptionalString > hash_tag_res;
        redis_cluster.mget( { "key{tag}1", "key{tag}2", "key{tag}3" }, std::back_inserter( hash_tag_res ) );
    }
    catch ( const Error& e )
    {
        // Error handling.
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
