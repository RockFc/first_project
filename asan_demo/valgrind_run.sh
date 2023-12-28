#!/bin/bash
#valgrind工具用于检测内存泄漏等问题工具使用脚本
if [ $# -eq 0 ]; then
    echo "Usage: $0 <executable>"
    exit 1
fi

valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$1

exit 0
