# 打印当前目录
message("源程序工程当前目录: "  ${CMAKE_CURRENT_SOURCE_DIR})

# 头文件目录
INCLUDE_DIRECTORIES(
    /usr/local/include/
    /usr/include
)

AUX_SOURCE_DIRECTORY(. src)

#生成静态库
#add_library(src ${src})

#生成可执行程序
add_executable(s_src_main ${src})