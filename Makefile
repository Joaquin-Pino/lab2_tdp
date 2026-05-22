CXX      = g++
CXXFLAGS = -std=c++17 -Wall

GRAFO_SRC        = grafo/grafo.cpp
CAMINO_SRC       = camino/camino.cpp
SOLVER_GREEDY_SRC = solverGreedy/solverGreedy.cpp
SOLVER_2OPT_SRC   = 2opt/solver2opt.cpp


all: testGrafo testCamino testSolverGreedy

testGrafo: $(GRAFO_SRC) grafo/testGrafo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testCamino: $(GRAFO_SRC) $(CAMINO_SRC) camino/testCamino.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testSolverGreedy: $(GRAFO_SRC) $(CAMINO_SRC) $(SOLVER_GREEDY_SRC) solverGreedy/testSolverGreedy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

main: $(GRAFO_SRC) $(CAMINO_SRC) $(SOLVER_GREEDY_SRC) main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f main testGrafo testCamino testSolverGreedy
