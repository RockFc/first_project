#include <iostream>
#include <modbus/modbus.h>

int main()
{
    // 创建一个 Modbus TCP 服务端
    modbus_t*         ctx = modbus_new_tcp("127.0.0.1", 1502);
    modbus_mapping_t* mb_mapping;

    // 创建一个 Modbus 映射，用于存储寄存器
    mb_mapping = modbus_mapping_new(0, 0, 100, 100);
    if (mb_mapping == NULL)
    {
        std::cerr << "Failed to allocate the mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // 绑定服务端并开始监听
    int server_socket = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &server_socket);

    while (true)
    {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int     rc;

        rc = modbus_receive(ctx, query);
        if (rc > 0)
        {
            modbus_reply(ctx, query, rc, mb_mapping);
        }
        else if (rc == -1)
        {
            break;
        }
    }

    // 释放资源
    modbus_mapping_free(mb_mapping);
    modbus_free(ctx);

    return 0;
}