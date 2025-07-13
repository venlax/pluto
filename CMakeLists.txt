cmake_minimum_required(VERSION 3.22)
project(pluto)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_compile_definitions(CONFIG_PATH="/home/venicebitch/pluto/config/test/test.yaml")

add_executable(pluto main.cpp
        src/log/log.cpp
        include/log/error.h
        include/common/errc_define.h
        src/config/config.cpp
        include/common/util.h
)
add_executable(test_sink test/log/test_sink.cpp
        src/log/log.cpp)


target_link_options(test_sink PRIVATE "-Wl,-rpath,/opt/gcc-15.1/lib64")

add_executable(test_log test/log/test_log.cpp src/log/log.cpp)
target_link_options(test_log PRIVATE "-Wl,-rpath,/opt/gcc-15.1/lib64")

add_executable(test_config test/config/test_config.cpp src/config/config.cpp)
target_link_libraries(test_config /usr/local/lib/libyaml-cpp.so dl)
target_link_options(test_config PRIVATE "-Wl,-rpath,/opt/gcc-15.1/lib64")

add_executable(test_pool test/thread/test_pool.cpp)
target_link_libraries(test_pool /usr/local/lib/libyaml-cpp.so dl)
target_link_options(test_pool PRIVATE "-Wl,-rpath,/opt/gcc-15.1/lib64")
