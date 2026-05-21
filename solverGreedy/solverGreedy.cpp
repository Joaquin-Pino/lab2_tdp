#include "solverGreedy.h"

SolverGreedy::SolverGreedy() : grafo(nullptr){}

SolverGreedy::SolverGreedy(const Grafo& grafo) : grafo(&grafo){}

int SolverGreedy::obtenerMejorNodo(int idNodoActual, int pesoDisponible, const Camino& sol){
    std::vector<Nodo> vecinos = grafo->getVecinos(idNodoActual);

    int idMejor = -1;
    float maxRatio = -1.0f;

    for (const Nodo& n : vecinos){
        if (n.costo == 0 || n.costo > pesoDisponible || sol.nodoFueVisitado(n.destino)) continue;
        float ratio = (float) n.beneficio / n.costo;
        if (ratio > maxRatio){
            maxRatio = ratio;
            idMejor = n.destino;
        }
    }

    return idMejor;
}

Camino SolverGreedy::resolver(){
    
    int idActual = 0;
    int pesoMax = grafo->getMaxW();
    Camino sol({}, *grafo);

    while (idActual != grafo->getCantVert() -1){
        sol.agregarNodo(idActual);
        int mejorId = obtenerMejorNodo(idActual, pesoMax, sol);
         
        if (mejorId == -1){
            return sol; // devolvemos la mejor solucion encontrada hasta ese momento
        }

        Nodo arista = grafo->getArista(idActual, mejorId);
        pesoMax -= arista.costo;
        idActual = mejorId;
       
    }

    sol.agregarNodo(idActual); // para agregar al ultimo nodo de la sol
    return sol;
}