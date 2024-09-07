cmake_minimum_required(VERSION 3.15)

project(despacer VERSION "0.1.0" LANGUAGES C)

# for consistency with windows
set(CMAKE_STATIC_LIBRARY_PREFIX_C "")

# despacer.lib
add_library(despacer STATIC "./src/despacer.c")
target_include_directories(despacer PRIVATE "./include")
set_target_properties(despacer PROPERTIES POSITION_INDEPENDENT_CODE ON) #-fPIC
target_compile_features(despacer PRIVATE "c_std_99")

message(STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR  "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL AMD64)
   if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "compiling with -march=native")
    target_compile_options(despacer PRIVATE -march=native)
  elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Assuming AVX2, SSE4.2, SSE4.1 support on MSVC")
    target_compile_options(despacer PRIVATE /arch:AVX2)
    target_compile_definitions(despacer PRIVATE __SSE4_1__ __SSE4_2__ __AVX2__)
  endif()
endif()

# despacer_unit.exe
option(UNIT_TESTS "Enable Unit tests" ON)
if(UNIT_TESTS)
    enable_testing()
    add_executable(despacer_unit "./tests/unit.c")
    target_include_directories(despacer_unit PRIVATE "./include")
    target_link_libraries(despacer_unit PRIVATE despacer)
    add_test(NAME unit COMMAND despacer_unit)
endif()


option(SANITIZER "Enable Sanitizer" OFF)
option(CMAKE_SANITIZER "Sanitizer to use" "address")
if(SANITIZER)
  target_compile_options(despacer PRIVATE -fsanitize=${CMAKE_SANITIZER})
  target_link_options(despacer PRIVATE -fsanitize=${CMAKE_SANITIZER})
endif()

option(BENCHMARKS "Enable Benchmarks" ON)
if(BENCHMARKS)
    enable_testing()
    message(STATUS "CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")
    if(("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL AMD64) AND ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") )
      add_executable(despacer_benchmark "./benchmarks/despacebenchmark.c")
      target_compile_options(despacer_benchmark PRIVATE -march=native)
      target_include_directories(despacer_benchmark PRIVATE "include")
      target_link_libraries(despacer_benchmark PRIVATE despacer)
      add_test(NAME benchmark COMMAND despacer_benchmark)
      target_compile_features(despacer PRIVATE "c_std_11")
    endif()
endif()
