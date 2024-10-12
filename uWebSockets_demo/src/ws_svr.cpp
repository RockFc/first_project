#include "App.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <thread>

/* ws->getUserData returns one of these */
struct PerSocketData
{
};

void runWebSocketServer()
{
    /* Very simple WebSocket echo server */
    uWS::App()
        .ws<PerSocketData>("/*", {/* Settings */
                                  .compression      = uWS::SHARED_COMPRESSOR,
                                  .maxPayloadLength = 16 * 1024,
                                  .idleTimeout      = 10,
                                  .maxBackpressure  = 1 * 1024 * 1024,
                                  /* Handlers */
                                  .upgrade = nullptr,
                                  .open =
                                      [](auto* /*ws*/)
                                  {

                                  },
                                  .message =
                                      [](auto* ws, std::string_view message, uWS::OpCode opCode)
                                  {
                                      std::cout << "recv mesgage: " << message << std::endl;
                                      ws->send(message, opCode);
                                  },
                                  .drain =
                                      [](auto* /*ws*/)
                                  {
                                      /* Check getBufferedAmount here */
                                  },
                                  .ping =
                                      [](auto* /*ws*/, std::string_view)
                                  {

                                  },
                                  .pong =
                                      [](auto* /*ws*/, std::string_view)
                                  {

                                  },
                                  .close =
                                      [](auto* /*ws*/, int /*code*/, std::string_view /*message*/)
                                  {

                                  }})
        .get("/",
             [](auto* res, auto* /*req*/)
             {
                 res->end("Hello uWebsocket!");
             })
        .get("/get_json",
             [](auto* res, auto* /*req*/)
             {
                 nlohmann::json j;
                 j["id"]   = "27";
                 j["name"] = "rock";
                 res->writeHeader("Access-Control-Allow-Origin", "*");
                 res->writeHeader("Content-Type", "application/json");
                 res->end(j.dump());
             })
        .post("/echo_json",
              [](auto* res, auto* req)
              {
                  /* Display the headers */
                  //   std::cout << " --- " << req->getUrl() << " --- " << std::endl;
                  //   for (auto [key, value] : *req)
                  //   {
                  //       std::cout << key << ": " << value << std::endl;
                  //   }

                  auto isAborted = std::make_shared<bool>(false);
                  auto body      = std::make_shared<std::string>();
                  res->onData(
                      [res, isAborted, body](std::string_view chunk, bool isFin) mutable
                      {
                          if (chunk.length())
                          {
                              body->append(chunk.data(), chunk.length());
                          }

                          if (isFin && !*isAborted)
                          {
                              try
                              {
                                  auto parsed_json = nlohmann::json::parse(*body);
                                  res->writeHeader("Content-Type", "application/json");
                                  res->end(parsed_json.dump(2));
                              }
                              catch (const std::exception& e)
                              {
                                  res->writeStatus("400 Bad Request");
                                  res->end(e.what());
                              }
                          }
                      });

                  res->onAborted(
                      [isAborted]()
                      {
                          *isAborted = true;
                          std::cout << "Request aborted" << std::endl;
                      });
              })
        .listen(10080,
                [](auto* listen_socket)
                {
                    if (listen_socket)
                    {
                        std::cout << "Thread " << std::this_thread::get_id()
                                  << " listening on port " << 10086 << std::endl;
                    }
                    else
                    {
                        std::cout << "Thread " << std::this_thread::get_id()
                                  << " failed to listen on port 10080" << std::endl;
                    }
                })
        .run();
}

int main()
{
    /* Simple echo websocket server, using multiple threads */
    std::vector<std::thread*> threads(std::thread::hardware_concurrency());

    std::transform(threads.begin(), threads.end(), threads.begin(),
                   [](std::thread* /*t*/)
                   {
                       return new std::thread(
                           []()
                           {
                               runWebSocketServer();
                           });
                   });

    std::for_each(threads.begin(), threads.end(),
                  [](std::thread* t)
                  {
                      t->join();
                      delete t;
                  });
}
