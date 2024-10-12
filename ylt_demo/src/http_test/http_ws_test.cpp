#include <iostream>

#include "ylt/coro_http/coro_http_client.hpp"
#include "ylt/coro_http/coro_http_server.hpp"

using namespace std::chrono_literals;
using namespace coro_http;

async_simple::coro::Lazy<void> use_websocket() {
  coro_http_server server(1, 9001);
  server.set_http_handler<GET>(
      "/ws_echo",
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
            std::cout << result.data << "\n";
          }
          else if (result.type == ws_frame_type::WS_PING_FRAME ||
                   result.type == ws_frame_type::WS_PONG_FRAME) {
            // ping pong frame just need to continue, no need echo anything,
            // because framework has reply ping/pong msg to client
            // automatically.
            continue;
          }
          else {
            // error frame
            break;
          }

          auto ec = co_await req.get_conn()->write_websocket(result.data);
          if (ec) {
            break;
          }
        }
      });
  server.async_start();
  std::this_thread::sleep_for(300ms);  // wait for server start

  coro_http_client client{};
  auto r = co_await client.connect("ws://127.0.0.1:9001/ws_echo");
  if (r.net_err) {
    co_return;
  }

  auto result = co_await client.write_websocket("hello websocket");
  assert(!result.net_err);
  auto data = co_await client.read_websocket();
  assert(data.resp_body == "hello websocket");
  
  result = co_await client.write_websocket("test again");
  assert(!result.net_err);
  data = co_await client.read_websocket();
  assert(data.resp_body == "test again");
}


async_simple::coro::Lazy<void> basic_usage() {
  coro_http_server server(1, 9001);
  server.set_http_handler<GET>(
      "/get", [](coro_http_request &req, coro_http_response &resp) {
        resp.set_status_and_content(status_type::ok, "ok");
      });

  server.async_start();
  std::this_thread::sleep_for(300ms);  // wait for server start

  coro_http_client client{};
  auto result = co_await client.async_get("http://127.0.0.1:9001/get");
  assert(result.status == 200);
  assert(result.resp_body == "ok");
  std::cout << "http rep:" << result.resp_body << "\n";
}

int main() {
  // async_simple::coro::syncAwait(basic_usage());
  async_simple::coro::syncAwait(use_websocket());

  return 0;
}

/**
 * 结合使用 HTTP 和 WebSocket

优点：

	1.	分工明确：
	•	HTTP 用于处理常规的 API 请求，如数据的获取和提交，适合请求-响应模式。
	•	WebSocket 用于处理实时通信，如通知推送、实时更新，适合长连接的场景。
	2.	灵活性：
	•	可以根据不同的需求选择合适的通信方式，既保证了实时性，又兼顾了常规请求的简单高效。
	3.	简化开发：
	•	HTTP 的请求-响应模式简单直观，适合大多数的 CRUD 操作，而 WebSocket 则用于处理需要持续连接的场景，如实时通知。

缺点：

	1.	两种协议管理：
	•	需要同时管理两种不同的协议，增加了系统的复杂度。
	2.	连接数量：
	•	在某些情况下，可能会同时维护大量的 HTTP 连接和 WebSocket 连接，对服务器的资源消耗更大。
 * **/