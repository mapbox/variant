CXX := $(CXX)
BOOST_LIBS = -lboost_timer -lboost_system -lboost_chrono
CXXFLAGS := $(CXXFLAGS) -O3 -Wall -Wsign-compare -DNDEBUG -Wsign-conversion -Wshadow -Wunused-parameter -pedantic -finline-functions -fvisibility-inlines-hidden -std=c++11
LDFLAGS := $(LDFLAGS)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++
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

test-variant: Makefile main.cpp variant.hpp
	$(CXX) -o test-variant main.cpp -I./ $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) $(BOOST_LIBS)

sizes: Makefile variant.hpp
	@$(CXX) -o /tmp/variant.out variant.hpp $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h /tmp/variant.out
	@$(CXX) -o /tmp/boost-variant.out $(RUN_ARGS) $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h /tmp/boost-variant.out
	@$(CXX) -o ./test/variant ./test/variant.cpp -I./ $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./test/variant
	@$(CXX) -o ./test/boost-variant ./test/boost-variant.cpp -I./ $(COMMON_FLAGS) $(CXXFLAGS) &&  du -h ./test/boost-variant

test: test-variant
	./test-variant 5000000

clean:
	rm -f ./test-variant
	rm -f ./test/variant
	rm -f ./test/boost-variant

.PHONY: sizes
