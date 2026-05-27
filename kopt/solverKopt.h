#pragma once
#include "../grafo/grafo.h"
#include "../camino/camino.h"
#include "../algoritmos/algoritmo.h"
#include <vector>

class SolverKopt{
private:
    Camino camino; // cuidado con sobreescribir el camino cuando se hace swap, hacer copia o deshacer el movimiento
    const Grafo* grafo;
    int k;
    
    void construirCaminoFaltante();
    
    bool swapValido(int idAnterior, int id, int posterior); // es decir el nodo intercambiado conecta con los otros nodos

    //tomamos la lista de elementos a cambiar en el camino y obtenemos los huecos que se forman
    std::vector<std::pair<int,int>> obtenerHuecos(std::vector<int> candidatos); // esto para optimizar permutaciones

public:
    SolverKopt();
    SolverKopt(int k,const Grafo& grafo, Camino c);

    Camino resolver();
    Camino resolverSalto();

    void setK(int k);
    int getK() const;
};