# variant

An alternative to boost::variant for C++11.

# Depends

 - Compiler supporting `-std=c++11`
 - Boost built with `--with-timer`

# Building

To build set your boost includes and libs locations and run `make`:

    export LDFLAGS='-L/opt/boost/lib'
    export CXXFLAGS='-I/opt/boost/include'
    make

# Benchmark

Run the benchmark like:

    make test

# Check object sizes

    make sizes /path/to/boost/variant.hpp