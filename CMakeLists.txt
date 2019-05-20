cmake_minimum_required(VERSION 3.14)
project(chat_server)

set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost REQUIRED COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
#add_definitions(-DBOOST_ASIO_ENABLE_HANDLER_TRACKING)

# Threads
find_package(Threads REQUIRED)

# Source and include dirs
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Source and header files
file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${INCLUDE_DIR}/*.hpp)

add_executable(chat_server ${SRC_FILES} ${HEADER_FILES})

target_include_directories(chat_server PUBLIC ${INCLUDE_DIR})

target_link_libraries(chat_server
        Threads::Threads
        ${Boost_SYSTEM_LIBRARY})