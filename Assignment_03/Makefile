CXX      := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -Iinclude

SRC_COMMON := src/csr_graph.cpp src/kruskal_mst.cpp src/prim_mst.cpp
OBJ_COMMON := $(SRC_COMMON:.cpp=.o)

all: mst_driver graph_generator

mst_driver: src/main.cpp $(OBJ_COMMON)
	$(CXX) $(CXXFLAGS) -o $@ $^

graph_generator: src/graph_generator.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f mst_driver graph_generator src/*.o

.PHONY: all clean
