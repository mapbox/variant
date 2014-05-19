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

all: test-variant

test-variant-debug: Makefile main.cpp variant.hpp
	$(CXX) -o test-variant-debug main.cpp -I./ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS)

debug: test-variant-debug

test-variant: Makefile main.cpp variant.hpp
	$(CXX) -o test-variant main.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS)

sizes: Makefile variant.hpp
	@$(CXX) -o /tmp/variant.out variant.hpp $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h /tmp/variant.out
	@$(CXX) -o /tmp/boost-variant.out $(RUN_ARGS) $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h /tmp/boost-variant.out
	@$(CXX) -o ./test/variant ./test/variant.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./test/variant
	@$(CXX) -o ./test/boost-variant ./test/boost-variant.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./test/boost-variant

bench: test-variant
	./test-variant 5000000

profile: test-variant-debug
	mkdir -p profiling/
	rm -rf profiling/*
	iprofiler -timeprofiler -d profiling/ ./test-variant-debug 500000

clean:
	rm -f ./test-variant
	rm -f ./test-variant-debug
	rm -f ./test/variant
	rm -f ./test/boost-variant
	rm -rf *.dSYM

pgo:
	$(CXX) -o test-variant main.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS) -pg -fprofile-generate
	./test-variant 500000 >/dev/null 2>/dev/null
	$(CXX) -o test-variant main.cpp -I./ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS) -fprofile-use

.PHONY: sizes
