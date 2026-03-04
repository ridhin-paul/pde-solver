cmake_minimum_required(VERSION 3.5)
project(pde_solver)

set(CMAKE_CXX_STANDARD 23)


include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)
FetchContent_MakeAvailable(Catch2)


add_subdirectory(src)


enable_testing()
add_subdirectory(tests)