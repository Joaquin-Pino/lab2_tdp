#include <cassert>
#include <iostream>
#include "../solverGreedy/solverGreedy.h"

// Grafo de prueba:
// 0->1 (costo=2, beneficio=3) ratio=1.5
// 0->2 (costo=4, beneficio=1) ratio=0.25
// 1->2 (costo=1, beneficio=2) ratio=2.0
// 1->3 (costo=3, beneficio=5) ratio=1.67
// 2->3 (costo=2, beneficio=4) ratio=2.0
// maxW=10, destino=3

Grafo crearGrafo() {
    Grafo g(4, 5, 10);
    g.insertarArista(0, 1, 2, 3);
    g.insertarArista(0, 2, 4, 1);
    g.insertarArista(1, 2, 1, 2);
    g.insertarArista(1, 3, 3, 5);
    g.insertarArista(2, 3, 2, 4);
    return g;
}

void test_llega_al_destino() {
    Grafo g = crearGrafo();
    SolverGreedy solver(g);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    assert(cam.front() == 0);
    assert(cam.back() == 3);
    std::cout << "test_llega_al_destino: OK\n";
}

void test_no_supera_peso() {
    Grafo g = crearGrafo();
    SolverGreedy solver(g);
    Camino sol = solver.resolver();

    assert(sol.getPesoTotal() <= g.getMaxW());
    std::cout << "test_no_supera_peso: OK\n";
}

void test_sin_nodos_repetidos() {
    Grafo g = crearGrafo();
    SolverGreedy solver(g);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    std::unordered_set<int> vistos;
    for (int id : cam) {
        assert(vistos.find(id) == vistos.end());
        vistos.insert(id);
    }
    std::cout << "test_sin_nodos_repetidos: OK\n";
}

void test_elige_por_ratio() {
    // Desde nodo 0: opcion A ratio=3.0, opcion B ratio=0.5
    // El greedy debe ir por A
    // 0->1 (costo=1, beneficio=3) ratio=3.0
    // 0->2 (costo=2, beneficio=1) ratio=0.5
    // 1->3 (costo=1, beneficio=1)
    // 2->3 (costo=1, beneficio=1)
    Grafo g(4, 4, 10);
    g.insertarArista(0, 1, 1, 3);
    g.insertarArista(0, 2, 2, 1);
    g.insertarArista(1, 3, 1, 1);
    g.insertarArista(2, 3, 1, 1);

    SolverGreedy solver(g);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    assert(cam[1] == 1); // debe haber elegido nodo 1 (mayor ratio)
    std::cout << "test_elige_por_ratio: OK\n";
}

void test_peso_insuficiente() {
    // maxW muy bajo, no puede llegar al destino
    Grafo g(4, 5, 1);
    g.insertarArista(0, 1, 2, 3);
    g.insertarArista(0, 2, 4, 1);
    g.insertarArista(1, 2, 1, 2);
    g.insertarArista(1, 3, 3, 5);
    g.insertarArista(2, 3, 2, 4);

    SolverGreedy solver(g);
    Camino sol = solver.resolver();

    // No llega al destino, pero el camino parcial debe ser valido
    assert(sol.getPesoTotal() <= 1);
    std::cout << "test_peso_insuficiente: OK\n";
}

int main() {
    test_llega_al_destino();
    test_no_supera_peso();
    test_sin_nodos_repetidos();
    test_elige_por_ratio();
    test_peso_insuficiente();
    std::cout << "--- Todos los tests de SolverGreedy pasaron ---\n";
    return 0;
}
