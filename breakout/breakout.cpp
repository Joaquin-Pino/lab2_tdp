#include "breakout.h"
#include "../solverGreedy/solverGreedy.h"

SolverBreakout::SolverBreakout() : grafo(nullptr), mejorCamino(Camino()), mejorBeneficio(0){}

SolverBreakout::SolverBreakout(const Grafo& grafo) : grafo(&grafo), mejorCamino(Camino()), mejorBeneficio(0) {}


void SolverBreakout::marcarVisitado(const Camino& cam){
    std::string caminoString = toString(cam.getCamino());
    visitados.insert(caminoString);
}

std::string SolverBreakout::toString(const std::vector<int>& v){
    std::string s;
    for (int x : v) s += std::to_string(x) + ",";
    return s;
}

bool SolverBreakout::estaVisitado(const Camino& camino){
   return visitados.count(toString(camino.getCamino())) > 0;
}

Camino SolverBreakout::resolver(){
    SolverGreedy greedy(*grafo);
    Camino camino = greedy.resolver();
    
    SolverKopt kopt(2, *grafo, camino);

    mejorCamino = kopt.resolver();
    mejorBeneficio = mejorCamino.getBeneficioTotal();
    marcarVisitado(mejorCamino);

    int kMax = (int)mejorCamino.getCamino().size() - 2;
    int sinMejora = 0;
    while (sinMejora < 200){
        kopt.resolverSalto();
        Camino caminoRefinado = kopt.resolver();

        if (estaVisitado(caminoRefinado)){
            if (kopt.getK() < kMax) kopt.setK(kopt.getK() + 1);
            sinMejora++;
        } else {
            marcarVisitado(caminoRefinado);
            if (caminoRefinado.getBeneficioTotal() > mejorBeneficio){
                mejorBeneficio = caminoRefinado.getBeneficioTotal();
                mejorCamino = caminoRefinado;
                sinMejora = 0;
            } else {
                sinMejora++;
            }
        }
    }

    return mejorCamino;
}