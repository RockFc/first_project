#include <iostream>
#include <thread>

#include "ylt/coro_http/coro_http_client.hpp"
#include "ylt/coro_http/coro_http_server.hpp"
#include "ylt/struct_json/json_reader.h"
#include "ylt/struct_json/json_writer.h"
#include "http_ws_define.h"

using namespace std::chrono_literals;
using namespace coro_http;

int main() {
    coro_http_server server(1, 9001);
    server.set_http_handler<GET>(
      "/get", [](coro_http_request &req, coro_http_response &resp) {
        GetInfo i{10, "rock"};
        std::string ss;
        struct_json::to_json(i, ss); // 序列化
        resp.set_status_and_content(status_type::ok, ss);
      });

      server.set_http_handler<GET>(
      "/ws_notify",
      [](coro_http_request &req,
         coro_http_response &resp) -> async_simple::coro::Lazy<void> {
        assert(req.get_content_type() == content_type::websocket);
        websocket_result result{};
        while (true) {
          result = co_await req.get_conn()->read_websocket();
          if (result.ec) {
            break;
          }

          if (result.type == ws_frame_type::WS_CLOSE_FRAME) {
            std::cout << "close frame\n";
            break;
          }

          if (result.type == ws_frame_type::WS_TEXT_FRAME ||
              result.type == ws_frame_type::WS_BINARY_FRAME) {
            std::cout << "recved data from ws client: " << result.data << "\n";
          }
          else if (result.type == ws_frame_type::WS_PING_FRAME ||
                   result.type == ws_frame_type::WS_PONG_FRAME) {
            // ping pong frame just need to continue, no need echo anything,
            // because framework has reply ping/pong msg to client
            // automatically.
            continue;
          }
          else {
            std::cout << "error frame!" << "\n";
            // error frame
            break;
          }
          
          std::this_thread::sleep_for(1s);
          StatusInfo si(0, "ok");
          std::string ss;
          struct_json::to_json(si, ss); // 序列化
          auto ec = co_await req.get_conn()->write_websocket(ss);
          if (ec) {
            break;
          }
        }
      });

    server.async_start();
    getchar();  // 保持程序运行等待回调完成

    return 0;
}

/*
	•	http://localhost:9001/get：应显示 “{"id":10,"name":"rock"}”。
*/