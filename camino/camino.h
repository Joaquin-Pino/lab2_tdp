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

    int calcularPesoTotal();
    int calcularBeneficioTotal();

public:
    Camino();
    Camino(std::vector<int> camino);

    int getPesoTotal();
    int getBeneficioTotal();

    bool nodoFueVisitado(int id);

    void intercambiarNodos(int id1, int id2);

    bool verificarCamino(int wMax); // verifica si camino es valido

    void agregarNodo(int id);
    void eliminarNodo(int id, const Grafo& grafo);

    int getUltimoNodo();

    std::vector<int> getCamino();
};