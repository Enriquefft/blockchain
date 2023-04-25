cmake_minimum_required(VERSION 3.26)
project(blockChain)



find_package(OpenSSL REQUIRED)



target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)

