cmake_minimum_required(VERSION 3.25)

add_library(BlockChain ${CMAKE_CURRENT_SOURCE_DIR}/BlockChain.cpp)

target_sources(
  BlockChain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Data.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/IndexStructures/Heap/Heap.cpp)

# Include include/BlockChainFloder
get_filename_component(INCLUDE_FOLDER ../../include ABSOLUTE)

target_include_directories(
  BlockChain PUBLIC ${INCLUDE_FOLDER}/BlockChain
                    ${INCLUDE_FOLDER}/BlockChain/IndexStructures)
target_include_directories(BlockChain PRIVATE ${INCLUDE_FOLDER})

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
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++98-compat-pedantic>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-zero-as-null-pointer-constant>
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
