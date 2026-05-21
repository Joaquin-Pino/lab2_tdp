CXX      = g++
CXXFLAGS = -std=c++17 -Wall

GRAFO_SRC  = grafo/grafo.cpp
CAMINO_SRC = camino/camino.cpp


all: testGrafo testCamino

testGrafo: $(GRAFO_SRC) grafo/testGrafo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testCamino: $(GRAFO_SRC) $(CAMINO_SRC) camino/testCamino.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

main: $(GRAFO_SRC) $(CAMINO_SRC) main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f main testGrafo testCamino
