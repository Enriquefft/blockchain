#!/bin/zsh
is_sourced(){
   if [ -n "$ZSH_VERSION" ]; then 
       case $ZSH_EVAL_CONTEXT in *:file:*) return 0;; esac
   else  # Add additional POSIX-compatible shell names here, if needed.
       case ${0##*/} in dash|-dash|bash|-bash|ksh|-ksh|sh|-sh) return 0;; esac
   fi
   echo "returning 1"
   return 1  # NOT sourced.
  }

 # Sample call.
is_sourced && sourced=1 || sourced=0

CXX=clang++ cmake -S . -B build
cmake --build build


if [ $sourced = 1 ]; then
  echo "exporting opetions"
  export LSAN_OPTIONS="strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:verbose=1:symbolize=1:log_threads=1"
else
  echo "Run with source to get the ASAN_OPTIONS environment variable"
fi



