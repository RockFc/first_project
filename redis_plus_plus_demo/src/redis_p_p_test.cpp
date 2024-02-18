#include "sw/redis++/redis++.h"
#include <iostream>

using namespace sw::redis;

int main()
{

    // try {
    // Create an Redis object, which is movable but NOT copyable.
    auto r = Redis( "tcp://127.0.0.1:6379" );
    r.auth( "123456" );

    // ***** STRING commands *****

    r.set( "key", "val" );
    auto val = r.get( "key" );  // val is of type OptionalString. See 'API Reference' section for details.
    if ( val )
    {
        // Dereference val to get the returned value of std::string type.
        std::cout << *val << std::endl;
    }  // else key doesn't exist.

    // } catch (const Error &e) {
    //     // Error handling.
    // }
    return 0;
}