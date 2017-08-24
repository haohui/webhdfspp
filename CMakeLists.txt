cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

enable_testing()

find_package(Threads)
find_package(CURL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic --std=c++11")

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -O2 -fPIC")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

add_definitions(-DASIO_HEADER_ONLY)

include_directories(
    include
    third_party/rapidjson/include
    third_party/asio-1.10.6/include
    ${CURL_INCLUDE_DIRS}
)

add_subdirectory(lib/webhdfspp)
add_subdirectory(tools)
