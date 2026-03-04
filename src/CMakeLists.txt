add_library(pde_solver_lib
        pde_solver.h
        pde_solver_cartesian.cpp
        pde_solver_cartesian.h
        pde_solver_polar.cpp
        pde_solver_polar.h
        io.cpp
        io.h
)


target_include_directories(pde_solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(pde_solver
        main.cpp
        io.cpp
        io.h
)


target_link_libraries(pde_solver PRIVATE pde_solver_lib)


# Add the frame pointer flag
target_compile_options(pde_solver_lib PRIVATE -fno-omit-frame-pointer)


target_compile_options(pde_solver PRIVATE -fno-omit-frame-pointer)