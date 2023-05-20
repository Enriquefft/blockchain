cmake_minimum_required(VERSION 3.26)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(openssl openssl/main.cpp)

set(GENERATOR_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:MSVC>:/W4
    /WX>
    $<$<CXX_COMPILER_ID:Clang>:-Weverything>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall
    -Wextra
    -Wpedantic
    -Werror>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++98-compat>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
    "-fsanitize=address,undefined"
    "-std=c++20")

#
add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "${GENERATOR_COMPILE_OPTIONS}"
  VERBATIM)

target_compile_options(openssl PRIVATE ${GENERATOR_COMPILE_OPTIONS})

# link sanitize
target_link_options(
  openssl PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

target_compile_features(openssl PRIVATE cxx_std_20)

find_package(OpenSSL REQUIRED)
target_link_libraries(openssl OpenSSL::Crypto)
