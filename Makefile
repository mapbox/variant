CXX := $(CXX)
BOOST_LIBS = -lboost_timer -lboost_system -lboost_chrono
RELEASE_FLAGS = -O3 -DNDEBUG -finline-functions -march=native -DSINGLE_THREADED
DEBUG_FLAGS = -O0 -g -DDEBUG -fno-inline-functions
COMMON_FLAGS = -Wall -Wsign-compare -Wsign-conversion -Wshadow -Wunused-parameter -pedantic -fvisibility-inlines-hidden -std=c++11
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++ -F/ -framework CoreFoundation
else
    BOOST_LIBS += -lrt
endif

ifeq (sizes,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
  ifndef RUN_ARGS
  $(error sizes target requires you pass full path to boost variant.hpp)
  endif
  .PHONY: $(RUN_ARGS)
endif

all: out/bench-variant

out:
	mkdir -p ./out

out/bench-variant-debug: out Makefile benchmark/test.cpp variant.hpp
	$(CXX) -o out/bench-variant-debug benchmark/test.cpp -I./ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS)

debug: out/bench-variant-debug

out/bench-variant: out Makefile benchmark/test.cpp variant.hpp
	$(CXX) -o out/bench-variant benchmark/test.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS)

sizes: out Makefile variant.hpp
	@$(CXX) -o ./out/variant_hello_world.out variant.hpp $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./out/variant_hello_world.out
	@$(CXX) -o ./out/boost_variant_hello_world.out $(RUN_ARGS) $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./out/boost_variant_hello_world.out
	@$(CXX) -o ./out/variant_hello_world ./test/variant_hello_world.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./out/variant_hello_world
	@$(CXX) -o ./out/boost_variant_hello_world ./test/boost_variant_hello_world.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./out/boost_variant_hello_world

bench: out/bench-variant
	./out/bench-variant 5000000

profile: out/bench-variant-debug
	mkdir -p profiling/
	rm -rf profiling/*
	iprofiler -timeprofiler -d profiling/ ./out/bench-variant-debug 500000

clean:
	rm -rf ./out
	rm -rf *.dSYM

pgo: out Makefile variant.hpp
	$(CXX) -o out/bench-variant benchmark/test.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS) -pg -fprofile-generate
	./test-variant 500000 >/dev/null 2>/dev/null
	$(CXX) -o out/bench-variant benchmark/test.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS) -fprofile-use

.PHONY: sizes
