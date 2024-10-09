#include <iostream>
#include <modbus/modbus.h>

int main()
{
    // 创建一个 Modbus TCP 客户端
    modbus_t* ctx = modbus_new_tcp("127.0.0.1", 1502);

    if (modbus_connect(ctx) == -1)
    {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // 读保持寄存器 (Holding Registers)
    uint16_t tab_reg[32];
    int      rc = modbus_read_registers(ctx, 0, 10, tab_reg);
    if (rc == -1)
    {
        std::cerr << "Failed to read registers: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // 打印寄存器值
    for (int i = 0; i < rc; i++)
    {
        std::cout << "Reg[" << i << "]=" << tab_reg[i] << std::endl;
    }

    // 断开连接并释放资源
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}