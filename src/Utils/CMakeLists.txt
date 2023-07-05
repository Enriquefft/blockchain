cmake_minimum_required(VERSION 3.25)

add_library(Utils)

target_sources(
  Utils
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Deque/Deque.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/ForwardList/ForwardList.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/RadixTrie/RadixTrie.cpp)

# Include include/BlockChainFloder
get_filename_component(INCLUDE_FOLDER ../../include ABSOLUTE)

target_include_directories(Utils PUBLIC ${INCLUDE_FOLDER}/Utils)

target_compile_options(
  Utils
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
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c++20-compat>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-zero-as-null-pointer-constant>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-error=padded>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
          $<$<CONFIG:RELEASE>:-Ofast>
          $<$<CONFIG:DEBUG>:-O0>
          $<$<CONFIG:DEBUG>:-ggdb3>
          "-fsanitize=address,undefined")

target_link_options(
  Utils PUBLIC $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

target_compile_features(Utils PUBLIC cxx_std_20)
