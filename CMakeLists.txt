cmake_minimum_required(VERSION 3.15)

project(despacer VERSION "0.1.0" LANGUAGES C)

# for consistency with windows
set(CMAKE_STATIC_LIBRARY_PREFIX_C "")

# despacer.lib
add_library(despacer STATIC "./src/lib.c")
target_include_directories(despacer INTERFACE "./include")
set_target_properties(despacer PROPERTIES POSITION_INDEPENDENT_CODE ON) #-fPIC

# std
set_target_properties(despacer PROPERTIES C_STANDARD 99)
