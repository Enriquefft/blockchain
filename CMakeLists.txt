cmake_minimum_required(VERSION 3.26)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cachimbo_coin src/main.cpp)

add_subdirectory(src/BlockChain)

target_include_directories(cachimbo_coin PUBLIC include)

get_target_property(cachimbo_coin_INCLUDE_DIRS cachimbo_coin
                    INTERFACE_INCLUDE_DIRECTORIES)
message(
  STATUS "main in_interface_include_directories: ${cachimbo_coin_INCLUDE_DIRS}")

get_target_property(cachimbo_coin_INCLUDE_DIRS cachimbo_coin
                    INCLUDE_DIRECTORIES)
message(STATUS "main in_include_directories: ${cachimbo_coin_INCLUDE_DIRS}")

target_link_libraries(cachimbo_coin PRIVATE BlockChain)

target_compile_features(cachimbo_coin PRIVATE cxx_std_20)
