cmake_minimum_required(VERSION 3.26)

# find_package(OpenSSL REQUIRED)
project(
  DataStructures
  VERSION 1.0.0
  DESCRIPTION "Robust implementation of a blockchain data structure"
  LANGUAGES CXX)

add_executable(ssl_test ssl_test.cpp)

# fsanitize
target_compile_options(ssl_test PRIVATE -fsanitize=address,undefined)

# link sanitize
target_link_options(ssl_test PRIVATE -fsanitize=address,undefined)

find_package(OpenSSL REQUIRED)
target_link_libraries(ssl_test OpenSSL::Crypto)
