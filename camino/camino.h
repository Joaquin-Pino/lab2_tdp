#pragma once
#include <vector>
#include <unordered_set>
#include "../grafo/grafo.h"

class Camino {
private:
    int pesoTotal;
    int beneficioTotal;
    std::unordered_set<int> visitados; // guarda id de nodos visitados, tabla hash por lo que consulta es o(1)
    std::vector<int> camino;

    int calcularPesoTotal(const Grafo& g);
    int calcularBeneficioTotal(const Grafo& g);

public:
    Camino();
    Camino(std::vector<int> camino, const Grafo& grafo);

    int getPesoTotal();
    int getBeneficioTotal();

    bool nodoFueVisitado(int id);
    bool marcarNodoVisitado(int id);

    void intercambiarNodos(int id1, int id2);

    bool verificarCamino(int wMax); // verifica si camino es valido

    void agregarNodo(int id, const Grafo& grafo);
    void eliminarNodo(int id, const Grafo& grafo);

    int getUltimoNodo();

    std::vector<int> getCamino();
};