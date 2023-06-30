cmake_minimum_required(VERSION 3.25)

project(
  cachimbo_coin
  VERSION 1.0.0
  DESCRIPTION "Cachimbo Coin"
  LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cachimbo_coin src/test.cpp)

add_subdirectory(src/BlockChain)
add_subdirectory(src/Utils)
add_subdirectory(src/Api)

target_include_directories(cachimbo_coin PRIVATE include/BlockChain)
target_include_directories(cachimbo_coin PRIVATE include/Utils)
# target_include_directories(cachimbo_coin PRIVATE include/Utils)

target_link_libraries(cachimbo_coin PRIVATE BlockChain)
target_link_libraries(cachimbo_coin PRIVATE Utils)
target_link_libraries(cachimbo_coin PRIVATE Api)

target_compile_features(cachimbo_coin PRIVATE cxx_std_20)
