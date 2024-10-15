#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义结构体
struct MyStruct
{
    int    intValue;
    char*  stringValue;
    double doubleValue;
};

// 序列化函数：将结构体序列化为字节流
// 返回值是序列化后的字节流，dataSize 是序列化数据的大小
void* serialize(const MyStruct* myStruct, size_t* dataSize)
{
    // 计算序列化数据的总大小
    size_t stringLength = strlen(myStruct->stringValue);
    *dataSize           = sizeof(myStruct->intValue) + sizeof(size_t) + stringLength
                + sizeof(myStruct->doubleValue);

    // 分配内存存储序列化的数据
    void* data = malloc(*dataSize);
    if (!data)
    {
        return NULL;  // 分配失败
    }

    // 使用 memcpy 进行序列化
    size_t offset = 0;

    // 复制 int
    memcpy(( char* )data + offset, &myStruct->intValue, sizeof(myStruct->intValue));
    offset += sizeof(myStruct->intValue);

    // 复制 string 的长度和内容
    memcpy(( char* )data + offset, &stringLength, sizeof(size_t));
    offset += sizeof(size_t);
    memcpy(( char* )data + offset, myStruct->stringValue, stringLength);
    offset += stringLength;

    // 复制 double
    memcpy(( char* )data + offset, &myStruct->doubleValue, sizeof(myStruct->doubleValue));

    return data;
}

// 反序列化函数：从字节流中重建结构体
// 返回值是反序列化后的结构体
MyStruct deserialize(const void* data, size_t dataSize)
{
    MyStruct myStruct;
    size_t   offset = 0;

    // 使用 memcpy 进行反序列化
    // 反序列化 int
    memcpy(&myStruct.intValue, ( char* )data + offset, sizeof(myStruct.intValue));
    offset += sizeof(myStruct.intValue);

    // 反序列化 string 的长度和内容
    size_t stringLength;
    memcpy(&stringLength, ( char* )data + offset, sizeof(size_t));
    offset += sizeof(size_t);

    myStruct.stringValue = ( char* )malloc(stringLength + 1);
    if (myStruct.stringValue)
    {
        memcpy(myStruct.stringValue, ( char* )data + offset, stringLength);
        myStruct.stringValue[stringLength] = '\0';  // 添加字符串结束符
    }
    offset += stringLength;

    // 反序列化 double
    memcpy(&myStruct.doubleValue, ( char* )data + offset, sizeof(myStruct.doubleValue));

    return myStruct;
}

// 打印结构体的函数
void printMyStruct(const MyStruct* myStruct)
{
    if (!myStruct)
    {
        printf("MyStruct is NULL\n");
        return;
    }
    printf("MyStruct values:\n");
    printf("intValue: %d\n", myStruct->intValue);
    printf("stringValue: %s\n", myStruct->stringValue);
    printf("doubleValue: %f\n", myStruct->doubleValue);
}

void doTest()
{
    // 创建一个结构体实例
    MyStruct original = {42, "Hello, World!", 3.14159};

    // 序列化结构体
    size_t dataSize;
    void*  serializedData = serialize(&original, &dataSize);
    if (!serializedData)
    {
        printf("Failed to serialize data.\n");
        return;
    }

    printf("Serialized data size: %zu bytes\n", dataSize);

    // 反序列化结构体
    MyStruct deserialized = deserialize(serializedData, dataSize);
    printf("Deserialized values:\n");
    printMyStruct(&deserialized);

    // 释放分配的内存
    free(serializedData);
    free(deserialized.stringValue);
}