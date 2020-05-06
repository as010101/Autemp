




#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
SET(CMAKE_C_COMPILER "/borealis/gcc/bin/gcc-4.1.1")
SET(CMAKE_CXX_COMPILER "/borealis/gcc/bin/g++-4.1.1")
# 项目信息
project (Demo3)


# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加 math 子目录
add_subdirectory(PredicatesAndExpressions)
# 指定生成目标
add_executable(Demo ${DIR_SRCS})
# 添加链接库
target_link_libraries(Demo MathFunctions)
