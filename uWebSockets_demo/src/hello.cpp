#include "App.h"

/* Note that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support */

int main()
{
    /* Overly simple hello world app */
    uWS::SSLApp(
        {.key_file_name = "misc/key.pem", .cert_file_name = "misc/cert.pem", .passphrase = "1234"})
        .get("/*",
             [](auto* res, auto* /*req*/)
             {
                 res->end("Hello world!");
             })
        .listen(10080,
                [](auto* listen_socket)
                {
                    if (listen_socket)
                    {
                        std::cout << "Listening on port " << 10086 << std::endl;
                    }
                })
        .run();

    std::cout << "Failed to listen on port 10086" << std::endl;
}
