cmake_minimum_required(VERSION 3.26)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(openssl openssl/main.cpp)

target_compile_features(
  openssl PRIVATE cxx_std_20
  )

# fsanitize
target_compile_options(openssl PRIVATE -fsanitize=address,undefined)

# link sanitize
target_link_options(openssl PRIVATE -fsanitize=address,undefined)

find_package(OpenSSL REQUIRED)
target_link_libraries(openssl OpenSSL::Crypto)
