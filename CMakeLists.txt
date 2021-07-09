cmake_minimum_required(VERSION 3.15)

project(despacer VERSION "0.1.0" LANGUAGES C)

# for consistency with windows
set(CMAKE_STATIC_LIBRARY_PREFIX_C "")

# despacer.lib
add_library(despacer STATIC "./src/despacer.c")
target_include_directories(despacer PRIVATE "./include")
set_target_properties(despacer PROPERTIES POSITION_INDEPENDENT_CODE ON) #-fPIC
target_compile_features(despacer PRIVATE "c_std_99")

if(MSVC)
    target_compile_options(despacer PRIVATE /arch:native)
else()
    target_compile_options(despacer PRIVATE -march=native)
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
