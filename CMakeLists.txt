cmake_minimum_required(VERSION 3.25)

project(
  testing
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(testing test.cpp)

target_compile_features(testing PRIVATE cxx_std_20)
