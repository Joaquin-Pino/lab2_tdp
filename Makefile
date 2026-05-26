CXX      = g++
CXXFLAGS = -std=c++17 -Wall

GRAFO_SRC        = grafo/grafo.cpp
CAMINO_SRC       = camino/camino.cpp
SOLVER_GREEDY_SRC = solverGreedy/solverGreedy.cpp
SOLVER_2OPT_SRC   = 2opt/solver2opt.cpp
ALGORITMO_SRC     = algoritmos/algoritmo.cpp
SOLVER_KOPT_SRC   = kopt/solverKopt.cpp


all: testGrafo testCamino testSolverGreedy testAlgoritmo testSolverKopt

testGrafo: $(GRAFO_SRC) grafo/testGrafo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testCamino: $(GRAFO_SRC) $(CAMINO_SRC) camino/testCamino.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testSolverGreedy: $(GRAFO_SRC) $(CAMINO_SRC) $(SOLVER_GREEDY_SRC) solverGreedy/testSolverGreedy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testAlgoritmo: $(GRAFO_SRC) $(ALGORITMO_SRC) algoritmos/testAlgoritmo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

testSolverKopt: $(GRAFO_SRC) $(CAMINO_SRC) $(ALGORITMO_SRC) $(SOLVER_KOPT_SRC) $(SOLVER_2OPT_SRC) kopt/testSolverKopt.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

main: $(GRAFO_SRC) $(CAMINO_SRC) $(SOLVER_GREEDY_SRC) main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f main testGrafo testCamino testSolverGreedy testAlgoritmo testSolverKopt
