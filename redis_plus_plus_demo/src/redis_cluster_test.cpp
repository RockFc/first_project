#include "common.h"
#include <sw/redis++/redis++.h>

using namespace sw::redis;

/*
    * Redis Sentinel：
    * Redis Sentinel 是 Redis 官方提供的一种高可用性解决方案，用于监控和管理 Redis 实例的故障转移和故障恢复。
    * Sentinel 通常用于管理单个 Redis 主从复制的架构，它能够监控 Redis 主从节点的状态，自动发现和处理节点故障，并进行故障转移，确保 Redis 系统的高可用性。
    * Sentinel 主要适用于小规模的 Redis 部署，对于需要高可用性的单个 Redis 实例来说是一个很好的选择。

    * Redis Cluster：
    * Redis Cluster 是 Redis 官方提供的另一种高可用性解决方案，用于分布式部署的场景下，提供分布式数据存储和高可用性的支持。
    * Redis Cluster 是一个分布式的数据库集群解决方案，它将数据分片存储在多个节点上，实现了数据的水平扩展和负载均衡。
    * Redis Cluster 提供了自动的数据分片和数据复制功能，能够自动发现节点间的连接关系，并在节点故障时进行故障转移，从而保证整个集群的高可用性。
    * Redis Cluster 适用于大规模的 Redis 部署，特别是对于需要大规模存储和高性能读写操作的场景，是一个非常合适的选择。

    *综上所述，Redis Sentinel 和 Redis Cluster 是两种不同的 Redis 高可用性解决方案，适用于不同规模和不同需求的 Redis 部署场景。如果你只需要对单个 Redis 实例进行高可用性保护，那么 Redis Sentinel 是一个不错的选择；如果你需要构建一个大规模的分布式 Redis 集群，并且希望实现数据的水平扩展和负载均衡，那么 Redis Cluster 将是更合适的选择。
*/

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
