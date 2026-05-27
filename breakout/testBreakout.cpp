#include <cassert>
#include <iostream>
#include "breakout.h"
#include "../solverGreedy/solverGreedy.h"

Grafo crearGrafo() {
    Grafo g(6, 6, 15);
    g.insertarArista(0, 1, 2, 1);
    g.insertarArista(0, 3, 2, 5);
    g.insertarArista(1, 2, 2, 1);
    g.insertarArista(2, 5, 2, 1);
    g.insertarArista(3, 2, 1, 3);
    g.insertarArista(3, 5, 2, 4);
    return g;
}

void test_camino_valido() {
    Grafo g = crearGrafo();
    SolverBreakout solver(g);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    assert(cam.front() == 0);
    assert(cam.back() == g.getCantVert() - 1);
    assert(sol.getPesoTotal() <= g.getMaxW());
    std::cout << "test_camino_valido: OK\n";
}

void test_sin_nodos_repetidos() {
    Grafo g = crearGrafo();
    SolverBreakout solver(g);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    std::unordered_set<int> vistos;
    for (int id : cam) {
        assert(vistos.find(id) == vistos.end());
        vistos.insert(id);
    }
    std::cout << "test_sin_nodos_repetidos: OK\n";
}

void test_al_menos_tan_bueno_como_greedy() {
    Grafo g = crearGrafo();
    SolverGreedy greedy(g);
    Camino solGreedy = greedy.resolver();

    SolverBreakout solver(g);
    Camino solBreakout = solver.resolver();

    assert(solBreakout.getBeneficioTotal() >= solGreedy.getBeneficioTotal());
    std::cout << "test_al_menos_tan_bueno_como_greedy: OK\n";
}

int main() {
    test_camino_valido();
    test_sin_nodos_repetidos();
    test_al_menos_tan_bueno_como_greedy();
    std::cout << "--- Todos los tests de SolverBreakout pasaron ---\n";
    return 0;
}
