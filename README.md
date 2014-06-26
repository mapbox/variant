# Mapbox Variant

An alternative to `boost::variant` for C++11.

[![Build Status](https://secure.travis-ci.org/mapbox/variant.svg)](https://travis-ci.org/mapbox/variant)
[![Build status](https://ci.appveyor.com/api/projects/status/v9tatx21j1k0fcgy)](https://ci.appveyor.com/project/Mapbox/variant)

# Depends

 - Compiler supporting `-std=c++11`
 - Tested with g++-4.7, g++-4.8, clang++ 3.4, and Visual C++ Compiler November 2013 CTP

Note: get the VS "CTP" release at http://www.microsoft.com/en-us/download/details.aspx?id=41151

# Usage

There is nothing to build, just include `variant.hpp` and `recursive_wrapper.hpp` in your project.

# Tests

The tests depend on:

 - Boost headers (for benchmarking against `boost::variant`)
 - Boost built with `--with-timer` (used for benchmark timing)

On Unix systems set your boost includes and libs locations and run `make test`:

    export LDFLAGS='-L/opt/boost/lib'
    export CXXFLAGS='-I/opt/boost/include'
    make test

On windows do:

    vcbuild

## Benchmark

On Unix systems run the benchmark like:

    make bench

## Check object sizes

    make sizes /path/to/boost/variant.hpp
