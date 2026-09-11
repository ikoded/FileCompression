cmake_minimum_required(VERSION 3.16)
project(FileCompression LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(compression
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/File.cpp"   
    "${CMAKE_SOURCE_DIR}/src/Node.cpp"
    "${CMAKE_SOURCE_DIR}/src/HuffmanCode.cpp"
)

target_include_directories(compression PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)