cmake_minimum_required(VERSION 4.0)
project(pde_solver)

set(CMAKE_CXX_STANDARD 23)


add_library(pde_solver_lib
        src/pde-solver.cpp
        src/pde-solver.h
)

target_include_directories(pde_solver_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_executable(pde_solver src/main.cpp)
target_link_libraries(pde_solver PRIVATE pde_solver_lib)

#Catch2 for Unit Testing -- advised to use a package manager in that case pls edit this file (use find_package())
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

#Creating test executable
add_executable(run_tests tests/test.cpp)

target_link_libraries(run_tests PRIVATE pde_solver_lib Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(run_tests)