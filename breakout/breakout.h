#pragma once
#include "../grafo/grafo.h"
#include "../kopt/solverKopt.h"
#include <string>
#include <unordered_set>

class SolverBreakout {
private: 
    const Grafo* grafo;
    
    Camino mejorCamino;
    int mejorBeneficio;
    std::unordered_set<std::string> visitados;


    void marcarVisitado(const Camino& cam);
    bool estaVisitado(const Camino& camino);

    std::string toString(const std::vector<int>& v);
    
public:
    SolverBreakout();
    SolverBreakout(const Grafo& grafo);
    Camino resolver();
};