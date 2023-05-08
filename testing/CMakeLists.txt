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



#     target_compile_options(openssl PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
#   $<$<CXX_COMPILER_ID:Clang>:-Weverything>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++98-compat>
#          $<$<CONFIG:RELEASE>:-Ofast>
#        $<$<CONFIG:DEBUG>:-O0>
#        $<$<CONFIG:DEBUG>:-ggdb3>
# )

# GENERATOR_COMPILE_OPTIONS is a string of options that are passed to the compiler made using generator expressions
set(GENERATOR_COMPILE_OPTIONS 
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<CXX_COMPILER_ID:Clang>:-Weverything>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++98-compat>
         $<$<CONFIG:RELEASE>:-Ofast>
       $<$<CONFIG:DEBUG>:-O0>
       $<$<CONFIG:DEBUG>:-ggdb3>
)

# Trim ';'
set(GENERATOR_COMPILE_OPTIONS_PRINTED ${GENERATOR_COMPILE_OPTIONS}/;/)
message(STATUS "GENERATOR_COMPILE_OPTIONS: ${GENERATOR_COMPILE_OPTIONS_PRINTED}")
#
add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo
  "${GENERATOR_COMPILE_OPTIONS_PRINTED}" VERBATIM)


target_compile_options(openssl PRIVATE ${GENERATOR_COMPILE_OPTIONS})

# fsanitize
# target_compile_options(openssl PRIVATE -fsanitize=address,undefined)

# link sanitize
# target_link_options(openssl PRIVATE -fsanitize=address,undefined)

find_package(OpenSSL REQUIRED)
target_link_libraries(openssl OpenSSL::Crypto)