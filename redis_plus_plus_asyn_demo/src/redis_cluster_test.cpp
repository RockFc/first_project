#include "common.h"
#include <sw/redis++/async_redis++.h>

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

    *综上所述，Redis Sentinel 和 Redis Cluster 是两种不同的 Redis 高可用性解决方案，适用于不同规模和不同需求的 Redis 部署场景。如果你只需要对单个 Redis 实例进行高可用性保护，那么 Redis Sentinel
   是一个不错的选择；如果你需要构建一个大规模的分布式 Redis 集群，并且希望实现数据的水平扩展和负载均衡，那么 Redis Cluster 将是更合适的选择。
*/

int main()
{

    try
    {
        // ***** Redis Cluster *****

        // Create a AsyncRedisCluster object, which is movable but NOT copyable.
        auto async_cluster = AsyncRedisCluster( "tcp://127.0.0.1:7001" );

        Future< bool > set_res = async_cluster.set( "key", "val" );
        cout << "set_res.get(): " << set_res.get() << endl;

        Future< Optional< string > > get_res = async_cluster.get( "key" );

        auto mget_res = async_cluster.mget< std::vector< OptionalString > >( { "{hashtag}key1", "{hashhag}key2", "{hashtag}key3" } );

        unordered_map< string, string > m         = { { "a", "b" }, { "c", "d" } };
        Future< void >                  hmset_res = async_cluster.hmset( "hash", m.begin(), m.end() );

        // Create an AsyncRedis object with hash-tag, so that we can send commands that has no key.
        // It connects to Redis instance that holds the given key, i.e. hash-tag.
        auto             r        = async_cluster.redis( "hash-tag" );
        Future< string > ping_res = r.command< string >( "ping" );

        sleep( 2 );
    }
    catch ( const Error& e )
    {
        // Error handling.
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
