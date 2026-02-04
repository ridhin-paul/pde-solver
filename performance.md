# Performance Analysis
## Method
To evaluate the computational efficiency and scalability of the solver,
performance was benchmarked using two distinct high-stress scenarios. 
These scenarios were designed to push the limits of the solver by utilizing high-density meshes and large number of internal boundary conditions.

### Test Scenarios
__Cartesian Coordinate System:__  Evaluated using a large-scale grid ($N_x, N_y = 500$) to test memory throughput and
cache efficiency under heavy iteration loads and 80 inner boundary conditions. <br>
__Polar Coordinate System:__ Benchmarked to observe the impact of coordinate 
transformations on solver convergence and runtime with mesh size of $N_x, N_y = 200$.

### Benchmarking tools

1. __Internal timer:__  A custom C++ Timer class (utilizing std::chrono::steady_clock) was injected into core solver 
functions. This provided high-precision time measurements of the "hot loops" in milliseconds (ms),
specifically tracking the time spent per iteration.
2. __System-level tool:__ The Linux perf utility was used to profile the execution at the kernel level. This allowed for the identification of CPU bottlenecks

## Optimisations
1. function is_bc to vector 
2. precalculated values
3. loop transform

## Results

### Final result comparing before and after all the optimisations

The implemented optimizations yielded a significant performance increase, resulting in an __81%__ reduction in total runtime for the Cartesian solver and a __55.43%__ reduction for the Polar solver. <br>


![comparison](images/benchmark/comparison.png)

### Comparison of output from the perf tool

#### Cartesian solver

The transition from the initial baseline to the optimized implementation resulted in a drastic shift in the solver's execution profile. In the initial version (Before), 
the system was heavily bottlenecked by logic overhead, with 84.79% of the runtime consumed by the is_bc()  function and its associated iterator increments. 
By refactoring the boundary condition checks into (_isnt_bc) and applying optimizations, this overhead was eliminated. Consequently, the final profile (After) shows the CPU spending the majority of its cycles (68.13%) on core
PDE logic.

##### Before

![init_cs](images/benchmark/init_cs.svg) <br>

##### After

![final_cs](images/benchmark/final_cs.svg)

#### Polar solver

The initial profile (Before) showed that 49.98% of the total runtime was consumed by expensive modulo arithmetic (fmod), 
in addition to the 27.72% overhead from boundary searches.
By optimizing the periodic logic to remove these fmod calls and refactoring the boundary checks, the final implementation (After) successfully eliminated these arithmetic stalls. 
This resulted in a 55.43% reduction in runtime (2.24x speedup).


##### Before

![init_po](images/benchmark/init_po.svg) <br>

##### After

![final_po](images/benchmark/final_po.svg)
