CPP=clang++
CPPFLAGS=-Wall -std=c++20 -g
LD=clang++
LDFLAGS=/usr/local/lib/libunwind.a -Wl,--rpath=/usr/local/lib -g

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp, build/%.o, $(SRC))

all: build/cpjs

build/%.o: src/%.cpp
	@ mkdir -p build
	@ echo COMPILING C++ OBJECT $^
	@ $(CPP) $(CPPFLAGS) -c $^ -o $@

build/cpjs: $(OBJ)
	@ echo LINKING C++ EXECUTABLE $@
	@ $(LD) $(LDFLAGS) $^ -o $@

clean:
	@ echo CLEANING UP
	@ rm -rf build/cpjs build/*.o