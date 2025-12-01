//
// Created by Ridhin Paul on 28.11.25.
//


#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
#include "pde-solver.h"
#include <sstream>
#include <streambuf>
#include <catch2/catch_approx.hpp>

//sprint 1

//stream redirection - to accomodate std::cin inside the function.
struct InputRedirect {
    std::stringstream input_ss;
    std::streambuf* old_cin_buf;

    explicit InputRedirect(const std::string& input_data)
    : old_cin_buf(std::cin.rdbuf()) {
        // 1. Fill the stringstream with the desired input data
        input_ss.str(input_data);
        // 2. Redirect std::cin to read from the stringstream
        std::cin.rdbuf(input_ss.rdbuf());

        // Ensure std::cin is in a good state before running the test
        std::cin.clear();
    }

    ~InputRedirect() {
        // 3. Restore the original std::cin buffer
        std::cin.rdbuf(old_cin_buf);
    }
};


//Test cases for void set_mesh(int& N, int& M, double& Lx, double& Ly);

//The below test will be replaced as input via console maybe removed in future, hence brocken into sections.

TEST_CASE("set_mesh function test scenerios", "[set_mesh]")
{
    //The below run before ach section is run
    int nx = 0, ny = 0;
    double lx = 0.0, ly = 0.0;

    //Test 1 -> Happy path
    SECTION("Happy path -All expected input criteria met") {

        std::string input_data = "10 20 5.0 10.0\n";
        InputRedirect redirect(input_data);

        REQUIRE_NOTHROW(set_mesh(nx, ny, lx, ly));

        CHECK(nx == 10);
        CHECK(ny == 20);
        CHECK(lx == 5.0);
        CHECK(ly == 10.0);
    }

    //Test 2 -> exception for small nx and ny
    SECTION("Must throw exception when nx or ny is less than 2") {

        std::string input_data = "1 10 5.0 10.0\n";
        InputRedirect redirect(input_data);

        //The function is expected to throw runtime error
        REQUIRE_THROWS_WITH(set_mesh(nx, ny, lx, ly), "nx and ny must be more than 2");
    }

    //Test 3 -> exception for non numeric input
    SECTION("Throws exception for non numeric input") {

        std::string input_data = "x 10 5.0 10.0\n";
        InputRedirect redirect(input_data);

        //The function is expected to throw runtime error
        REQUIRE_THROWS_WITH(set_mesh(nx, ny, lx, ly), "Invalid input");
    }

    //Test 3 -> exception for domain length being negetive; can it be zero?
    SECTION("Throws exception for non-positive domain length") {

        std::string input_data = "10 10 -10.0 10.0\n";
        InputRedirect redirect(input_data);

        //The function is expected to throw runtime error
        REQUIRE_THROWS_WITH(set_mesh(nx, ny, lx, ly), "Domain lengths must be positive");
    }
}

//Test cases for void set_boundaries(std::vector<std::vector<double>>& U, int N, int M);

TEST_CASE("set_boundaries function correctly sets the 4 boundaries", "[set_boundaries][happy path]")
{

    //Defining fixed dimensions (constexpr suggested by ide)
    constexpr int nx = 10;
    constexpr int ny = 10;


    SECTION("Checks all the boundaries are set correctly: happy path 1") {

        //New mesh created for each section
        Mesh mesh(nx, std::vector<double>(ny, 0.0));


        //Setting values to be tested
        const double t_bot_val = 10.0;
        const double t_top_val = 20.0;
        const double t_left_val = 30.0;
        const double t_right_val = 40.0;

        std::string input_data = "10.0 20.0 30.0 40.0\n";
        InputRedirect redirect(input_data);

        REQUIRE_NOTHROW(set_boundaries(mesh, nx, ny));


        for (int j = 0; j < ny; j++)
            {
                CHECK(mesh[0][j] == t_bot_val);
            }

        for (int j = 0; j < ny; j++)
            {
                CHECK(mesh[nx - 1][j] == t_top_val);
            }

        for (int i = 1; i < nx - 1; i++)
            {
                CHECK(mesh[i][0] == t_left_val);
            }

        for (int i = 1; i < nx - 1; i++)
            {
                CHECK(mesh[i][ny - 1] == t_right_val);
            }
    }


    SECTION("Checks all the boundaries are set correctly: happy path 2") {

        Mesh mesh(nx, std::vector<double>(ny, 0.0));

        const double t_bot_val = 10.6;
        const double t_top_val = -20.0;
        const double t_left_val = 30.0;
        const double t_right_val = -40.5;

        std::string input_data = "10.6 -20.0 30.0 -40.5\n";
        InputRedirect redirect(input_data);

        REQUIRE_NOTHROW(set_boundaries(mesh, nx, ny));

        for (int j = 0; j < ny; j++)
        {
            CHECK(mesh[0][j] == t_bot_val);
        }

        for (int j = 0; j < ny; j++)
        {
            CHECK(mesh[nx - 1][j] == t_top_val);
        }

        for (int i = 1; i < nx - 1; i++)
        {
            CHECK(mesh[i][0] == t_left_val);
        }

        for (int i = 1; i < nx - 1; i++)
        {
            CHECK(mesh[i][ny - 1] == t_right_val);
        }
    }

}

//Testing just for one invalid input
TEST_CASE("set_boundaries function calls out invalid input", "[set_boundaries][invalid input]")
{
    constexpr  int nx = 10;
    constexpr int ny = 10;

    Mesh mesh(nx, std::vector<double>(ny, 0.0));


    std::string input_data = "o -20.0 30.0 -40.5\n";
    InputRedirect redirect(input_data);

    REQUIRE_THROWS_WITH(set_boundaries(mesh, nx, ny), "Invalid input");
}


//Test cases for void solve_steady_state(std::vector<std::vector<double>>& U, int N, int M, int max_iter = 1000, double tolerance = 1e-3);


TEST_CASE("solve_steady_state function compared to analytical solution", "[solve_steady_state][happy path]")
{
    constexpr  int nx = 5;
    constexpr int ny = 5;
    constexpr int max_iter = 1000;
    constexpr double tol_solver = 1.0e-6;

    Mesh mesh(nx, std::vector<double>(ny, 0.0));

    double t_bot{5.0};
    double t_top{15.0};
    double t_left{10.0};
    double t_right{5.0};

    for (int i = 0; i < ny; i++) {
        mesh[0][i] = t_bot;
    }
    for (int i = 0; i < ny; i++) {
        mesh[nx - 1][i] = t_top;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][0] = t_left;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][ny - 1] = t_right;
    }

    solve_steady_state(mesh, nx, ny, max_iter, tol_solver);

    SECTION("Checks the solved interior nodes")
    {
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

        CHECK(T_11_exact == Catch::Approx(mesh[1][1]).margin(tol));
        CHECK(T_12_exact == Catch::Approx(mesh[1][2]).margin(tol));
        CHECK(T_13_exact == Catch::Approx(mesh[1][3]).margin(tol));

        CHECK(T_21_exact == Catch::Approx(mesh[2][1]).margin(tol));
        CHECK(T_22_exact == Catch::Approx(mesh[2][2]).margin(tol));
        CHECK(T_23_exact == Catch::Approx(mesh[2][3]).margin(tol));

        CHECK(T_31_exact == Catch::Approx(mesh[3][1]).margin(tol));
        CHECK(T_32_exact == Catch::Approx(mesh[3][2]).margin(tol));
        CHECK(T_33_exact == Catch::Approx(mesh[3][3]).margin(tol));


    }

    SECTION("Checks boundary integrity")
    {
        for (int j = 0; j < ny; j++)
        {
            CHECK(mesh[0][j] == t_bot);
        }

        for (int j = 0; j < ny; j++)
        {
            CHECK(mesh[nx - 1][j] == t_top);
        }

        for (int i = 1; i < nx - 1; i++)
        {
            CHECK(mesh[i][0] == t_left);
        }

        for (int i = 1; i < nx - 1; i++)
        {
            CHECK(mesh[i][ny - 1] == t_right);
        }
    }

}

TEST_CASE("solve_steady_state function: check for maximum iteration limit", "[solve_steady_state][max_iterLimit]")
{
    constexpr  int nx = 5;
    constexpr int ny = 5;
    constexpr int max_iter = 3;
    constexpr double tol_solver = 1.0e-8;

    Mesh mesh(nx, std::vector<double>(ny, 0.0));

    double t_bot{5.0};
    double t_top{15.0};
    double t_left{10.0};
    double t_right{5.0};

    for (int i = 0; i < ny; i++) {
        mesh[0][i] = t_bot;
    }
    for (int i = 0; i < ny; i++) {
        mesh[nx - 1][i] = t_top;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][0] = t_left;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][ny - 1] = t_right;
    }

    REQUIRE_THROWS_WITH(solve_steady_state(mesh, nx, ny, max_iter, tol_solver), "maximum iterations (" + std::to_string(max_iter) +") reached; required tolerance not achieved. ""Decrease mesh size or increase iteration limit.");
}
