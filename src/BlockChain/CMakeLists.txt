cmake_minimum_required(VERSION 3.25)

add_library(BlockChain ${CMAKE_CURRENT_SOURCE_DIR}/BlockChain.cpp)

set(CMAKE_CXX_STANDARD 20)

target_sources(BlockChain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Data.cpp)

# Include include/BlockChainFloder
get_filename_component(INCLUDE_FOLDER ../../include ABSOLUTE)

target_include_directories(BlockChain PUBLIC ${INCLUDE_FOLDER}/BlockChain)
target_include_directories(BlockChain PRIVATE ${INCLUDE_FOLDER})

target_include_directories(BlockChain SYSTEM PUBLIC io1-money/include)

# Message target_include_directories
get_target_property(BlockChain_INCLUDE_DIRS BlockChain
                    INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "in_interface_include_directories: ${BlockChain_INCLUDE_DIRS}")

# Get current cxx standard
get_target_property(MY_CXX_STANDARD BlockChain INTERFACE_COMPILE_FEATURES)
message(STATUS "cxx_standard: ${MY_CXX_STANDARD}")

get_target_property(BlockChain_INCLUDE_DIRS BlockChain INCLUDE_DIRECTORIES)
message(STATUS "in_include_directories: ${BlockChain_INCLUDE_DIRS}")

target_compile_options(
  BlockChain
  PRIVATE # Prefered warnings
          $<$<CXX_COMPILER_ID:MSVC>:/W4
          /WX>
          $<$<CXX_COMPILER_ID:Clang>:-Weverything>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall
          -Wextra
          -Wpedantic
          -Werror>
          # Disable some warnings
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++98-compat>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-error=padded>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
          $<$<CONFIG:RELEASE>:-Ofast>
          $<$<CONFIG:DEBUG>:-O0>
          $<$<CONFIG:DEBUG>:-ggdb3>
          "-fsanitize=address,undefined")

target_link_options(
  BlockChain PUBLIC
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

find_package(OpenSSL REQUIRED)
target_link_libraries(BlockChain OpenSSL::Crypto)

target_compile_features(BlockChain PUBLIC cxx_std_20)
