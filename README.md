# Safe Arena

This is a simple arena allocator showing it is possible to have a safe
interface while preserving the speed advantage. Allocation is templated, so
alignment and type are correct for callers without them violating the C++ Core
Guidelines (CCG) rules for type safety. Arrays of multiple objects are returned
as spans, so callers don't need to violate the CCG rules for bounds safety.
When lifetime safety annoations are released, one can get safety from
use-after-free bugs by adding [[lifetime(const)]] annotations to the allocation
member functions.

## Steps to run on Linux

        # requires cmake and a c++ compiler
        cd safe_area
        mkdir release
        cd release
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make
        ./benchmark_arena

## Steps to run on Windows

        :: requires cmake and visual studio
        cd safe_area
        mkdir build
        cd build
        cmake ..
        safe_arena.sln
        :: use visual studio's gui to build in release
        Release/benchmark_arena

## Performance

Execution time to make a simple binary tree structure:

|                 | std::unique_ptr + std::vector | abc::arena + gsl::span | speedup  |
| --------------- | ----------------------------- | ---------------------- | -------- |
| Linux (clang)   | 3143 ms                       | 1007 ms                | 3.12x    |
| Windows (msvc)  | 7621 ms                       | 1261 ms                | 6.04x    |

So using an arena makes this about 3x to 6x faster. Results between Linux and
Windows are not directly comparable, because although they were on the same
hardware, only Windows was in a virtual machine.
