#include <iostream>
#include <thread>

#include "ylt/coro_http/coro_http_client.hpp"
#include "ylt/coro_http/coro_http_server.hpp"

using namespace std::chrono_literals;
using namespace coro_http;

async_simple::coro::Lazy<void> ws_client_test() {
  coro_http_client client{};
  auto r = co_await client.connect("ws://127.0.0.1:9001/ws_notify");
  if (r.net_err) {
    co_return;
  }

  auto result = co_await client.write_websocket("hello websocket");
  assert(!result.net_err);

  while (true) {
    auto result = co_await client.read_websocket();
    if (result.net_err) {
      break;
    }
    // 处理 WebSocket 消息
    std::cout << "Received message for ws svr: " << result.resp_body << std::endl;
  }
}


async_simple::coro::Lazy<void> client_test() {
  coro_http_client client{};
  auto result = co_await client.async_get("http://127.0.0.1:9001/get");
  assert(result.status == 200);
  std::cout << "http rep:" << result.resp_body << "\n";
}

int main() {
  async_simple::coro::syncAwait(client_test());
  async_simple::coro::syncAwait(ws_client_test());
  return 0;
}