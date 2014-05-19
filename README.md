# MapBox Variant

An alternative to `boost::variant` for C++11.

# Depends

 - Compiler supporting `-std=c++11`
 - Tested with g++-4.7, g++-4.8, and clang++ 5.x

# Usage

There is nothing to build, just include `variant.hpp` and `recursive_wrapper.hpp` in your project.

# Tests

The tests depend on:

 - Boost headers (for benchmarking against `boost::variant`)
 - Boost built with `--with-timer` (used for benchmark timing)

Set your boost includes and libs locations and run `make test`:

    export LDFLAGS='-L/opt/boost/lib'
    export CXXFLAGS='-I/opt/boost/include'
    make test

## Benchmark

Run the benchmark like:

    make bench

## Check object sizes

    make sizes /path/to/boost/variant.hpp