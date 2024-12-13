#!/bin/bash
# 代码覆盖率统计
# 注意：测试程序运行后，再执行该脚本，则可以在指定路径生成测试报告
gcovr -r .  --exclude './third_party/' --html --html-details -o ./product/cov/coverage.html