
#include <atomic>
#include <unistd.h>

#include <ananas/net/Application.h>
#include <ananas/net/Connection.h>
#include <ananas/util/Logger.h>

std::shared_ptr< ananas::Logger > logger;

size_t OnMessage(ananas::Connection* conn, const char* data, size_t len)
{
    // echo package
    std::string rsp(data, len);
    conn->SendPacket(rsp.data(), rsp.size());
    return len;
}

void OnNewConnection(ananas::Connection* conn)
{
    using ananas::Connection;

    conn->SetOnMessage(OnMessage);
    conn->SetOnDisconnect([](Connection* conn) {
        WRN(logger) << "OnDisConnect " << conn->Identifier();
    });
}

int main(int ac, char* av[])
{
    size_t workers = 1;
    if (ac > 1)
        workers = ( size_t )std::stoi(av[ 1 ]);

    ananas::LogManager::Instance().Start();
    logger = ananas::LogManager::Instance().CreateLog(logALL, logALL, "logger_server_test");

    auto& app = ananas::Application::Instance();
    app.SetNumOfWorker(workers);
    app.Listen("127.0.0.1", 9987, OnNewConnection);

    app.Run(ac, av);

    return 0;
}