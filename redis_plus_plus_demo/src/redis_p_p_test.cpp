#include "common.h"
#include <sw/redis++/redis++.h>

using namespace sw::redis;

int main()
{

    try
    {
        // Create an Redis object, which is movable but NOT copyable.
        auto redis = Redis( "tcp://123456@127.0.0.1:6379" );

        // ***** STRING commands *****

        redis.set( "key", "val" );
        auto val = redis.get( "key" );  // val is of type OptionalString. See 'API Reference' section for details.
        if ( val )
        {
            // Dereference val to get the returned value of std::string type.
            std::cout << *val << std::endl;
        }  // else key doesn't exist.

        // ***** LIST commands *****

        // std::vector<std::string> to Redis LIST.
        std::vector< std::string > vec = { "a", "b", "c" };
        redis.del( "list" );
        redis.rpush( "list", vec.begin(), vec.end() );

        // std::initializer_list to Redis LIST.
        redis.rpush( "list", { "a", "b", "c" } );

        // Redis LIST to std::vector<std::string>.
        vec.clear();
        redis.lrange( "list", 0, -1, std::back_inserter( vec ) );

        show_vec( "list", vec );

        // ***** HASH commands *****

        redis.hset( "hash", "field", "val" );

        // Another way to do the same job.
        redis.hset( "hash", std::make_pair( "field", "val" ) );

        // std::unordered_map<std::string, std::string> to Redis HASH.
        std::unordered_map< std::string, std::string > m = { { "field1", "val1" }, { "field2", "val2" } };
        redis.hmset( "hash", m.begin(), m.end() );

        // Redis HASH to std::unordered_map<std::string, std::string>.
        m.clear();
        redis.hgetall( "hash", std::inserter( m, m.begin() ) );
        show_map( "hash", m );

        // Get value only.
        // NOTE: since field might NOT exist, so we need to parse it to OptionalString.
        std::vector< OptionalString > vals;
        redis.hmget( "hash", { "field1", "field2" }, std::back_inserter( vals ) );
        show_opt_vec( "hash_vals", vals );

        // ***** SET commands *****

        redis.sadd( "set", "m1" );

        // std::unordered_set<std::string> to Redis SET.
        std::unordered_set< std::string > set = { "m2", "m3" };
        redis.sadd( "set", set.begin(), set.end() );

        // std::initializer_list to Redis SET.
        redis.sadd( "set", { "m2", "m3" } );

        // Redis SET to std::unordered_set<std::string>.
        set.clear();
        redis.smembers( "set", std::inserter( set, set.begin() ) );
        show_vec( "set", set );

        if ( redis.sismember( "set", "m1" ) )
        {
            std::cout << "m1 exists" << std::endl;
        }  // else NOT exist.

        // ***** SORTED SET commands *****

        redis.zadd( "sorted_set", "m1", 1.3 );

        // std::unordered_map<std::string, double> to Redis SORTED SET.
        std::unordered_map< std::string, double > scores = { { "m2", 2.3 }, { "m3", 4.5 } };
        redis.zadd( "sorted_set", scores.begin(), scores.end() );

        // Redis SORTED SET to std::vector<std::pair<std::string, double>>.
        // NOTE: The return results of zrangebyscore are ordered, if you save the results
        // in to `std::unordered_map<std::string, double>`, you'll lose the order.
        std::vector< std::pair< std::string, double > > zset_result;
        redis.zrangebyscore( "sorted_set", UnboundedInterval< double >{},  // (-inf, +inf)
                             std::back_inserter( zset_result ) );

        // Only get member names:
        // pass an inserter of std::vector<std::string> type as output parameter.
        std::vector< std::string > without_score;
        redis.zrangebyscore( "sorted_set", BoundedInterval< double >( 1.5, 3.4, BoundType::CLOSED ),  // [1.5, 3.4]
                             std::back_inserter( without_score ) );

        // Get both member names and scores:
        // pass an back_inserter of std::vector<std::pair<std::string, double>> as output parameter.
        std::vector< std::pair< std::string, double > > with_score;
        redis.zrangebyscore( "sorted_set", BoundedInterval< double >( 1.5, 3.4, BoundType::LEFT_OPEN ),  // (1.5, 3.4]
                             std::back_inserter( with_score ) );

        // ***** SCRIPTING commands *****

        // Script returns a single element.
        auto num = redis.eval< long long >( "return 1", {}, {} );
        std::cout << "Evaluated number: " << num << std::endl;

        // Script returns an array of elements.
        std::vector< std::string > nums;
        redis.eval( "return {ARGV[1], ARGV[2]}", {}, { "1", "2" }, std::back_inserter( nums ) );

        // mset with TTL
        auto mset_with_ttl_script = R"(
        local len = #KEYS
        if (len == 0 or len + 1 ~= #ARGV) then return 0 end
        local ttl = tonumber(ARGV[len + 1])
        if (not ttl or ttl <= 0) then return 0 end
        for i = 1, len do redis.call("SET", KEYS[i], ARGV[i], "EX", ttl) end
        return 1
    )";

        // Set multiple key-value pairs with TTL of 60 seconds.
        auto                       keys = { "key1", "key2", "key3" };
        std::vector< std::string > args = { "val1", "val2", "val3", "60" };
        redis.eval< long long >( mset_with_ttl_script, keys.begin(), keys.end(), args.begin(), args.end() );

        // ***** Pipeline *****

        // Create a pipeline. pipeline is used to reduce RTT (Round Trip Time), and speed up Redis queries.
        auto pipe = redis.pipeline();

        // Send mulitple commands and get all replies.
        auto pipe_replies = pipe.set( "key", "value" ).get( "key" ).rename( "key", "new-key" ).rpush( "list", { "a", "b", "c" } ).lrange( "list", 0, -1 ).exec();

        // Parse reply with reply type and index.
        auto set_cmd_result = pipe_replies.get< bool >( 0 );

        auto get_cmd_result = pipe_replies.get< OptionalString >( 1 );

        // rename command result
        pipe_replies.get< void >( 2 );

        auto rpush_cmd_result = pipe_replies.get< long long >( 3 );

        std::vector< std::string > lrange_cmd_result;
        pipe_replies.get( 4, back_inserter( lrange_cmd_result ) );

        std::cout << "set_cmd_result: " << set_cmd_result << std::endl;
        std::cout << "get_cmd_result: " << get_cmd_result.value() << std::endl;
        std::cout << "rpush_cmd_result: " << rpush_cmd_result << std::endl;
        std::cout << "lrange_cmd_result: " << lrange_cmd_result.size() << std::endl;
        show_vec( "lrange_cmd_result", lrange_cmd_result );

        // ***** Transaction *****

        /*   
        * Redis 的事务（Transaction）是一组命令的有序执行序列，并且这些命令要么全部执行，要么全部不执行。事务通过 MULTI、EXEC、DISCARD 和 WATCH 这几个命令来实现。
        * Redis 事务的原子性是基于单个客户端的，也就是说在一个事务中的一系列命令要么全部执行，要么全部不执行，而不是基于多个客户端的。这意味着在一个事务中的命令在执行过程中是不会被其他客户端所干扰的，但并不意味着事务中的命令对其他事务是完全隔离的。
        * Redis 事务并不支持真正的回滚（rollback）。如果在执行事务期间发生了错误，事务中已经执行的命令不会被回滚。但是，可以通过使用 DISCARD 命令来取消一个事务，该命令会清除事务队列中的所有命令，使事务回到初始状态。
        * 另外需要注意的是，使用 WATCH 命令可以实现乐观锁机制，以确保在事务执行期间，监视的键没有被其他客户端修改。如果被监视的键在 WATCH 和 EXEC 之间被修改，事务将被取消。
        * 综上所述，Redis 事务可以保证在单个客户端的一系列命令的原子性，但对于多个客户端之间的并发操作，并不能完全保证。
        */

        // Create a transaction.
        auto tx = redis.transaction();

        // Run multiple commands in a transaction, and get all replies.
        auto tx_replies = tx.incr( "num0" ).incr( "num1" ).mget( { "num0", "num1" } ).exec();

        // Parse reply with reply type and index.
        auto incr_result0 = tx_replies.get< long long >( 0 );

        auto incr_result1 = tx_replies.get< long long >( 1 );

        std::vector< OptionalString > mget_cmd_result;
        tx_replies.get( 2, back_inserter( mget_cmd_result ) );

        std::cout << "incr_result0: " << incr_result0 << std::endl;
        std::cout << "incr_result1: " << incr_result1 << std::endl;
        show_opt_vec( "mget_cmd_result", mget_cmd_result );
        redis.del( { "num0", "num1" } );

        // ***** Generic Command Interface *****

        // There's no *Redis::client_getname* interface.
        // But you can use *Redis::command* to get the client name.
        val = redis.command< OptionalString >( "client", "getname" );
        if ( val )
        {
            std::cout << "redis_client_getname:" << *val << std::endl;
        }

        // Same as above.
        auto getname_cmd_str = { "client", "getname" };
        val                  = redis.command< OptionalString >( getname_cmd_str.begin(), getname_cmd_str.end() );

        // There's no *Redis::sort* interface.
        // But you can use *Redis::command* to send sort the list.
        std::vector< std::string > sorted_list;
        redis.command( "sort", "list", "ALPHA", std::back_inserter( sorted_list ) );

        show_vec( "sorted_list", sorted_list );

        // Another *Redis::command* to do the same work.
        sorted_list.clear();
        auto sort_cmd_str = { "sort", "list", "ALPHA" };
        redis.command( sort_cmd_str.begin(), sort_cmd_str.end(), std::back_inserter( sorted_list ) );

        show_vec( "sorted_list", sorted_list );
    }
    catch ( const Error& e )
    {
        // Error handling.
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
