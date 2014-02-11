CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS) -O3 -std=c++11
LDFLAGS := $(LDFLAGS) -lboost_timer -lboost_system -lboost_chrono

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++
endif

all: test-variant

test-variant: Makefile main.cpp timer.hpp variant.hpp
	$(CXX) -o test-variant main.cpp $(CXXFLAGS) $(LDFLAGS) $(COMMON_FLAGS)

test:
	./test-variant 1000000