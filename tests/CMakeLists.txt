add_executable(run_tests
        test_S2.cpp
        test_S1.cpp
)


target_link_libraries(run_tests PRIVATE
        pde_solver_lib
        Catch2::Catch2WithMain
)


include(Catch)
catch_discover_tests(run_tests)