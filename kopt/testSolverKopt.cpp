#include <cassert>
#include <iostream>
#include <unordered_set>
#include "solverKopt.h"
#include "../2opt/solver2opt.h"

// Grafo de 6 nodos (destino=5), maxW=15:
//   0->1 (costo=2, beneficio=1)
//   0->3 (costo=2, beneficio=5)
//   1->2 (costo=2, beneficio=1)
//   2->5 (costo=2, beneficio=1)
//   3->2 (costo=1, beneficio=3)  -- 3 encaja en hueco (0,2)
//   3->5 (costo=2, beneficio=4)
//
// Camino inicial: [0,1,2,5], beneficio=3, peso=6
// Optimo k=1:    [0,3,2,5], beneficio=9, peso=5
Grafo crearGrafoK1() {
    Grafo g(6, 6, 15);
    g.insertarArista(0, 1, 2, 1);
    g.insertarArista(0, 3, 2, 5);
    g.insertarArista(1, 2, 2, 1);
    g.insertarArista(2, 5, 2, 1);
    g.insertarArista(3, 2, 1, 3);
    g.insertarArista(3, 5, 2, 4);
    return g;
}

// Grafo de 7 nodos (destino=6), maxW=20:
//   0->1, 1->2, 2->3, 3->6  (camino base, beneficio=4, peso=4)
//   0->5, 5->2               (5 encaja en hueco (0,2))
//   2->4, 4->6               (4 encaja en hueco (2,6))
//
// Camino inicial: [0,1,2,3,6], beneficio=4
// Optimo k=2:    [0,5,2,4,6], beneficio=8
Grafo crearGrafoK2() {
    Grafo g(7, 8, 20);
    g.insertarArista(0, 1, 1, 1);
    g.insertarArista(1, 2, 1, 1);
    g.insertarArista(2, 3, 1, 1);
    g.insertarArista(3, 6, 1, 1);
    g.insertarArista(0, 5, 1, 2);
    g.insertarArista(5, 2, 1, 2);
    g.insertarArista(2, 4, 1, 2);
    g.insertarArista(4, 6, 1, 2);
    return g;
}

void test_resolver_mejora_beneficio() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);
    SolverKopt solver(1, g, inicial);
    Camino sol = solver.resolver();

    assert(sol.getBeneficioTotal() > inicial.getBeneficioTotal());
    std::cout << "test_resolver_mejora_beneficio: OK\n";
}

void test_resolver_no_supera_peso() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);
    SolverKopt solver(1, g, inicial);
    Camino sol = solver.resolver();

    assert(sol.getPesoTotal() <= g.getMaxW());
    std::cout << "test_resolver_no_supera_peso: OK\n";
}

void test_resolver_llega_al_destino() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);
    SolverKopt solver(1, g, inicial);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    assert(cam.front() == 0);
    assert(cam.back() == g.getCantVert() - 1);
    std::cout << "test_resolver_llega_al_destino: OK\n";
}

void test_resolver_sin_nodos_repetidos() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);
    SolverKopt solver(1, g, inicial);
    Camino sol = solver.resolver();

    std::vector<int> cam = sol.getCamino();
    std::unordered_set<int> vistos;
    for (int id : cam) {
        assert(vistos.find(id) == vistos.end());
        vistos.insert(id);
    }
    std::cout << "test_resolver_sin_nodos_repetidos: OK\n";
}

void test_resolver_k2_mejora() {
    Grafo g = crearGrafoK2();
    Camino inicial({0, 1, 2, 3, 6}, g);
    SolverKopt solver(2, g, inicial);
    Camino sol = solver.resolver();

    assert(sol.getBeneficioTotal() > inicial.getBeneficioTotal());
    assert(sol.getPesoTotal() <= g.getMaxW());
    std::cout << "test_resolver_k2_mejora: OK\n";
}

void test_resolver_ya_optimo() {
    // [0,3,2,5] es el optimo local, resolver no debe empeorar
    Grafo g = crearGrafoK1();
    Camino optimo({0, 3, 2, 5}, g);
    SolverKopt solver(1, g, optimo);
    Camino sol = solver.resolver();

    assert(sol.getBeneficioTotal() >= optimo.getBeneficioTotal());
    assert(sol.getPesoTotal() <= g.getMaxW());
    std::cout << "test_resolver_ya_optimo: OK\n";
}

void test_salto_produce_camino_valido() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);
    SolverKopt solver(1, g, inicial);
    Camino sol = solver.resolverSalto();

    assert(sol.getPesoTotal() <= g.getMaxW());

    std::vector<int> cam = sol.getCamino();
    std::unordered_set<int> vistos;
    for (int id : cam) {
        assert(vistos.find(id) == vistos.end());
        vistos.insert(id);
    }
    std::cout << "test_salto_produce_camino_valido: OK\n";
}

// En un grafo con un unico swap posible, kopt k=1 y 2opt deben dar el mismo resultado.
// crearGrafoK1 tiene un solo nodo candidato valido para el hueco (0,2): el nodo 3.
void test_kopt_k1_igual_que_2opt() {
    Grafo g = crearGrafoK1();
    Camino inicial({0, 1, 2, 5}, g);

    SolverKopt kopt(1, g, inicial);
    Camino solKopt = kopt.resolver();

    Solver2opt solver2opt(g, inicial);
    Camino sol2opt = solver2opt.resolver();

    assert(solKopt.getCamino() == sol2opt.getCamino());
    assert(solKopt.getBeneficioTotal() == sol2opt.getBeneficioTotal());
    std::cout << "test_kopt_k1_igual_que_2opt: OK\n";
}

int main() {
    test_resolver_mejora_beneficio();
    test_resolver_no_supera_peso();
    test_resolver_llega_al_destino();
    test_resolver_sin_nodos_repetidos();
    test_resolver_k2_mejora();
    test_resolver_ya_optimo();
    test_salto_produce_camino_valido();
    test_kopt_k1_igual_que_2opt();
    std::cout << "--- Todos los tests de SolverKopt pasaron ---\n";
    return 0;
}
