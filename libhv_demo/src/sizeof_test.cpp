#include <stdio.h>

#include "hv/hloop.h"

#include "hv/EventLoop.h"
#include "hv/EventLoopThread.h"
#include "hv/EventLoopThreadPool.h"
#include "hv/Channel.h"
#include "hv/TcpClient.h"
#include "hv/TcpServer.h"
#include "hv/UdpClient.h"
#include "hv/UdpServer.h"

#include "hv/HttpMessage.h"
#include "hv/HttpContext.h"
#include "hv/HttpServer.h"
#include "hv/HttpResponseWriter.h"

#include "hv/WebSocketChannel.h"
#include "hv/WebSocketParser.h"
#include "hv/WebSocketServer.h"
#include "hv/WebSocketClient.h"

using namespace hv;

int main() {
    // evpp
    printf("sizeof(class EventLoop)=%lu\n", sizeof(EventLoop));
    printf("sizeof(class EventLoopThread)=%lu\n", sizeof(EventLoopThread));
    printf("sizeof(class EventLoopThreadPool)=%lu\n", sizeof(EventLoopThreadPool));
    printf("sizeof(class Channel)=%lu\n", sizeof(Channel));
    printf("sizeof(class SocketChannel)=%lu\n", sizeof(SocketChannel));
    printf("sizeof(class TcpClient)=%lu\n", sizeof(TcpClient));
    printf("sizeof(class TcpServer)=%lu\n", sizeof(TcpServer));
    printf("sizeof(class UdpClient)=%lu\n", sizeof(UdpClient));
    printf("sizeof(class UdpServer)=%lu\n", sizeof(UdpServer));
    // http
    printf("sizeof(class HttpRequest)=%lu\n", sizeof(HttpRequest));
    printf("sizeof(class HttpResponse)=%lu\n", sizeof(HttpResponse));
    printf("sizeof(class HttpContext)=%lu\n", sizeof(HttpContext));
    printf("sizeof(class HttpServer)=%lu\n", sizeof(HttpServer));
    printf("sizeof(class HttpResponseWrite)=%lu\n", sizeof(HttpResponseWriter));
    // websocket
    printf("sizeof(class WebSocketChannel)=%lu\n", sizeof(WebSocketChannel));
    printf("sizeof(class WebSocketParser)=%lu\n", sizeof(WebSocketParser));
    printf("sizeof(class WebSocketClient)=%lu\n", sizeof(WebSocketClient));
    printf("sizeof(class WebSocketServer)=%lu\n", sizeof(WebSocketServer));
    return 0;
}
