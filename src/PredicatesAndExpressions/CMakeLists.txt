
aux_source_directory(. DIR_LIB_SRCS)

SET(CMAKE_C_COMPILER "/borealis/gcc/bin/gcc-4.1.1")
SET(CMAKE_CXX_COMPILER "/borealis/gcc/bin/g++-4.1.1")

# 指定生成 MathFunctions 链接库

add_library (MathFunctions ${DIR_LIB_SRCS})
