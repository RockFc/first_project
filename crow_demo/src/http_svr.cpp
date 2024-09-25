#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    (
        []()
        {
            return "Hello, world!";
        });

    app.port(10080).run();
}
