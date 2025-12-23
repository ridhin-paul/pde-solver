cmake_minimum_required(VERSION 3.50)
project(pde_solver)

set(CMAKE_CXX_STANDARD 23)


add_library(pde_solver_lib
        src/pde_solver.h
        src/pde_solver_cartesian.cpp
        src/pde_solver_cartesian.h
)

target_include_directories(pde_solver_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_executable(pde_solver src/main.cpp
        src/io.cpp
        src/io.h
        src/pde_solver_cartesian.cpp
        src/pde_solver_cartesian.h)
target_link_libraries(pde_solver PRIVATE pde_solver_lib)

#Catch2 for Unit Testing - recomended to use a package manager, in that case pls edit this file (use find_package())
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(Catch2)


enable_testing()

#Creating test executable
add_executable(run_tests tests/test_S1.cpp)

target_link_libraries(run_tests PRIVATE pde_solver_lib Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(run_tests)