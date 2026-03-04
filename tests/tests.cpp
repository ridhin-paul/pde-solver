//
// Created by Ridhin Paul on 04.03.26.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "pde_solver.h"
#include "pde_solver_cartesian.h"
#include "pde_solver_polar.h"
#include "io.h"


//input parser

TEST_CASE("io::read_input function handles valid and invalid inputs", "[io][parsing]") {
    const std::string test_file = "test_config.ins";

    SECTION("Throws exception for non-numeric input") {
        std::ofstream out(test_file);
        out << "cs cartesian\n";
        out << "nx abc\n";
        out.close();

        REQUIRE_THROWS_WITH(io::read_input(test_file), "nx must be a positive integer");
    }

    SECTION("Throws exception for missing or unknown coordinate system") {
        std::ofstream out(test_file);
        out << "cs spherical\n";
        out.close();

        REQUIRE_THROWS_WITH(io::read_input(test_file), Catch::Matchers::ContainsSubstring("Choose the coordinate system"));
    }
}


//Cartesian solver tests


TEST_CASE("Cartesian solver initialization and boundaries", "[cartesian][init]") {
    inputConfig cfg;
    cfg.cs = inputConfig::CoordinateSystem::Cartesian;
    cfg.nx = 10;
    cfg.ny = 10;
    cfg.lx = 5.0;
    cfg.ly = 10.0;
    cfg.max_iter = 1000;
    cfg.tolerance = 1e-3;

    SECTION("Happy path - Expected input criteria met") {

        REQUIRE_NOTHROW(std::make_unique<pde_solver_cartesian>(cfg));
    }

    SECTION("Exception - nx or ny less than 2") {
        cfg.nx = 1;
        REQUIRE_THROWS_WITH(std::make_unique<pde_solver_cartesian>(cfg), "nx and ny must be more than 2");
    }

    SECTION("Exception - Non-positive domain length ") {
        cfg.lx = -10.0;
        REQUIRE_THROWS_WITH(std::make_unique<pde_solver_cartesian>(cfg), "Domain lengths must be positive");
    }

    SECTION("Boundary setup check: happy path 1") {
        cfg.t_bot = 10.0;
        cfg.t_top = 20.0;
        cfg.t_left = 30.0;
        cfg.t_right = 40.0;

        auto solver = std::make_unique<pde_solver_cartesian>(cfg);
        auto mesh = solver->getMesh();

        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(mesh[0][j] == cfg.t_left);
        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(mesh[cfg.nx - 1][j] == cfg.t_right);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(mesh[i][0] == cfg.t_bot);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(mesh[i][cfg.nx - 1] == cfg.t_top);

    }

    SECTION("Boundary setup check: happy path 2 with negatives") {
        cfg.t_bot = 10.6;
        cfg.t_top = -20.0;
        cfg.t_left = 30.0;
        cfg.t_right = -40.5;

        auto solver = std::make_unique<pde_solver_cartesian>(cfg);
        auto mesh = solver->getMesh();

        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(mesh[0][j] == cfg.t_left);
        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(mesh[cfg.nx - 1][j] == cfg.t_right);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(mesh[i][0] == cfg.t_bot);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(mesh[i][cfg.nx - 1] == cfg.t_top);
    }
}

TEST_CASE("Cartesian solver execution and exact comparisons", "[cartesian][solve]") {
    inputConfig cfg;
    cfg.cs = inputConfig::CoordinateSystem::Cartesian;
    cfg.nx = 5;
    cfg.ny = 5;
    cfg.lx = 1.0;
    cfg.ly = 1.0;
    cfg.t_bot = 5.0;
    cfg.t_top = 15.0;
    cfg.t_left = 10.0;
    cfg.t_right = 5.0;

    SECTION("Checks the solved interior nodes against analytical solution") {

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

        REQUIRE_NOTHROW(solver->solve());
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


        // Checks boundary values-> solving didn't change the boundary values
        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(resMesh[0][j] == cfg.t_left);
        for (int j = 1; j < cfg.ny - 1; j++)
            CHECK(resMesh[cfg.nx - 1][j] == cfg.t_right);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(resMesh[i][0] == cfg.t_bot);
        for (int i = 0; i < cfg.nx; i++)
            CHECK(resMesh[i][cfg.nx - 1] == cfg.t_top);
    }

    SECTION("Check for maximum iteration limit") {
        cfg.max_iter = 3;
        cfg.tolerance = 1.0e-8;
        auto solver = std::make_unique<pde_solver_cartesian>(cfg);

        REQUIRE_THROWS_WITH(solver->solve(), Catch::Matchers::ContainsSubstring("maximum iterations (3) reached; required tolerance not achieved."));
    }
}


//Polar solver tests

TEST_CASE("Polar solver test scenarios", "[polar]") {
    inputConfig tstcfg;
    tstcfg.cs = inputConfig::CoordinateSystem::Polar;
    tstcfg.max_iter = 10000;
    tstcfg.tolerance = 1e-8;
    tstcfg.t_out = 5.0;
    tstcfg.nr = 5;
    tstcfg.na = 5;
    tstcfg.lr = 1.0;

    SECTION("Happy path - Execution, boundary, and stability check") {
        std::unique_ptr<pde_solver> solver = std::make_unique<pde_solver_polar>(tstcfg);

        REQUIRE_NOTHROW(solver->solve());

        auto resMesh = solver->getMesh();

        for (int j = 0; j < tstcfg.na; j++) {
            CHECK(resMesh[tstcfg.nr - 1][j] == Catch::Approx(5.0));
        }

        for (int i = 0; i < tstcfg.nr - 1; i++) {
            for (int j = 0; j < tstcfg.na; j++) {
                CHECK(resMesh[i][j] >= 0.0);
                CHECK(resMesh[i][j] <= 5.0);
            }
        }
    }

    SECTION("Exception - Invalid mesh dimensions") {
        tstcfg.nr = 2;
        REQUIRE_THROWS_WITH(std::make_unique<pde_solver_polar>(tstcfg), "nr and na must be more than 2");
    }

    SECTION("Exception - Negative domain radius") {
        tstcfg.lr = -1.0;
        REQUIRE_THROWS_WITH(std::make_unique<pde_solver_polar>(tstcfg), "Domain radius must be positive");
    }
}

