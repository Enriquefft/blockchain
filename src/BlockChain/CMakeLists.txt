cmake_minimum_required(VERSION 3.26)

add_library(BlockChain BlockChain.cpp)

target_include_directories(BlockChain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/BlockChain)

# fsanitize
target_compile_options(BlockChain PRIVATE -fsanitize=address,undefined)

# link sanitize
target_link_options(BlockChain PRIVATE -fsanitize=address,undefined)

find_package(OpenSSL REQUIRED)
target_link_libraries(BlockChain OpenSSL::Crypto)
