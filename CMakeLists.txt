cmake_minimum_required(VERSION 3.26)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/BlockChain)

add_executable(cachimbo_coin src/main.cpp)
