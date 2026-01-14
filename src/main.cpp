#include <memory>
#include "pde_solver.h"
#include "pde_solver_cartesian.h"
#include "pde_solver_polar.h"

int main()
{


try
    {
        inputConfig cfg = io::read_input("config.ins");

        std::unique_ptr<pde_solver> solver;


        //dispatcher could be moved out of main depending on complexity
        switch (cfg.cs)
        {
            case inputConfig::CoordinateSystem::Cartesian:
                solver = std::make_unique<pde_solver_cartesian>(cfg);
                solver->solve();
                std::cout << "Solver finished, starting export." << std::endl;
                io::write_output(solver->_mesh, cfg, solver->_dx, solver->_dy);
                break;
            case inputConfig::CoordinateSystem::Polar:
                solver = std::make_unique<pde_solver_polar>(cfg);
                solver->solve();
                std::cout << "Solver finished, starting export." << std::endl;
                io::write_output(solver->_mesh, cfg, solver->_dx, solver->_dy, solver -> _center);
                break;
            default:
                throw std::runtime_error("Unknown input config");
        }
        return 0;
    }

  catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return -1;
  }

}
