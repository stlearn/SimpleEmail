# CMakeList.txt: Email 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("Email")

# 将源代码添加到此项目的可执行文件。
add_executable (Email "Email.cpp" "Email.h" "Menu/menu.cpp" "Menu/menu.h" "Receive.cpp" "Receive.h" "Send.cpp" "Send.h")

# TODO: 如有需要，请添加测试并安装目标。
