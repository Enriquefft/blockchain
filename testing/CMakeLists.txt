cmake_minimum_required(VERSION 3.25)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_executable(main main.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(main PRIVATE cxx_std_20)
