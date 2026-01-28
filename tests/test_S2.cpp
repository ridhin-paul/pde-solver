//
// Created by Ridhin Paul on 28.11.25.
//


#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include "pde_solver.h"
#include <sstream>
#include <streambuf>
#include <catch2/catch_approx.hpp>

#include "pde_solver_cartesian.h"
#include "pde_solver_polar.h"

//sprint 2

//The below test will be replaced as input via console maybe removed in future, hence brocken into sections.

TEST_CASE("Cartesian solver test scenerios", "[cartesian]") {
    int nx = 5, ny = 5;
    double lx = 1.0, ly = 1.0;

    constexpr  double t_bot_val = 5.0;
    constexpr double t_top_val = 15.0;
    constexpr double t_left_val = 10.0;
    constexpr double t_right_val = 5.0;

    inputConfig tstcfg;

    tstcfg.cs = inputConfig::CoordinateSystem::Cartesian;

    tstcfg.nx = nx;
    tstcfg.ny = ny;
    tstcfg.lx = lx;
    tstcfg.ly = ly;
    tstcfg.max_iter = 10000;
    tstcfg.tolerance = 1e-8;

    tstcfg.t_bot = t_bot_val;
    tstcfg.t_left = t_left_val;
    tstcfg.t_top = t_top_val;
    tstcfg.t_right = t_right_val;


    std::unique_ptr<pde_solver> solver = std::make_unique<pde_solver_cartesian>(tstcfg);

    //Test 1 -> Happy path
    SECTION("Happy path - Boundary condition check") {
        REQUIRE_NOTHROW(solver ->solve());

        auto resMesh = solver->getMesh();

        for (int i = 0; i < nx; i++) {
            CHECK(resMesh[i][0] == t_bot_val);
        }
        for (int i = 0; i < nx; i++) {
            CHECK(resMesh[i][nx - 1] == t_top_val);
        }
        for (int i = 1; i < ny - 1; i++) {
            CHECK(resMesh[0][i] == t_left_val);
        }
        for (int i = 1; i < ny - 1; i++) {
            CHECK(resMesh[nx - 1][i] == t_right_val);
        }
    }

    SECTION("Happy path - Analytical solution comparison") {

        REQUIRE_NOTHROW(solver ->solve());
        auto resMesh = solver->getMesh();

        constexpr  double tol = 1e-2;

        //Analytical solution
        //Not creating a result matrix for being more explicit
        constexpr  double T_11_exact = 7.8571;
        constexpr  double T_12_exact = 6.9196;
        constexpr  double T_13_exact = 6.0740;


        constexpr  double T_21_exact = 9.5089;
        constexpr  double T_22_exact = 8.7500;
        constexpr  double T_23_exact = 7.3661;


        constexpr  double T_31_exact = 11.4286;
        constexpr  double T_32_exact = 11.2054;
        constexpr  double T_33_exact = 9.6429;

        CHECK(T_11_exact == Catch::Approx(resMesh[1][1]).margin(tol));
        CHECK(T_21_exact == Catch::Approx(resMesh[1][2]).margin(tol));
        CHECK(T_31_exact == Catch::Approx(resMesh[1][3]).margin(tol));

        CHECK(T_12_exact == Catch::Approx(resMesh[2][1]).margin(tol));
        CHECK(T_22_exact == Catch::Approx(resMesh[2][2]).margin(tol));
        CHECK(T_32_exact == Catch::Approx(resMesh[2][3]).margin(tol));

        CHECK(T_13_exact == Catch::Approx(resMesh[3][1]).margin(tol));
        CHECK(T_23_exact == Catch::Approx(resMesh[3][2]).margin(tol));
        CHECK(T_33_exact == Catch::Approx(resMesh[3][3]).margin(tol));

    }
}


TEST_CASE("Polar solver test scenerios", "[polar]") {

    int nr = 5;
    int na = 5;

    double lr = 1.0;

    inputConfig tstcfg;

    tstcfg.cs = inputConfig::CoordinateSystem::Polar;

    tstcfg.max_iter = 10000;
    tstcfg.tolerance = 1e-8;

    tstcfg.t_out = 5;
    tstcfg.nr = nr;
    tstcfg.na = na;
    tstcfg.lr = lr;


    std::unique_ptr<pde_solver> solver = std::make_unique<pde_solver_polar>(tstcfg);
    SECTION("Happy path - Boundary condition and result check") {
        REQUIRE_NOTHROW(solver ->solve());

        auto resMesh = solver->getMesh();
        constexpr  double tol = 1e-2;

        for (const auto &val: resMesh) {
            for (const auto &row: val) {

                CHECK(5.0 == Catch::Approx(row).margin(tol));
            }
        }
    }
}