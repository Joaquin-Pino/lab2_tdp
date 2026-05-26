#include "solverKopt.h"
#include <algorithm>
#include <random>

SolverKopt::SolverKopt() : grafo(nullptr), k(0) {}

SolverKopt::SolverKopt(int k, const Grafo& grafo, Camino c) : camino(c), grafo(&grafo), k(k) {}

void SolverKopt::construirCaminoFaltante() {
    int ultimo = camino.getUltimoNodo();
    int destino = grafo->getCantVert() - 1;
    if (ultimo == destino) return;

    std::vector<int> resto = grafo->dijkstraCamino(ultimo, destino);
    if (resto.empty()) return;

    // verificar colisiones y costo extra antes de modificar el camino
    int costoExtra = 0;
    for (int i = 1; i < (int)resto.size(); i++) {
        if (camino.nodoFueVisitado(resto[i])) return;
        
        try {
            costoExtra += grafo->getArista(resto[i-1], resto[i]).costo;
        } catch (...) { return; } // arista no existe
    }

    if (camino.getPesoTotal() + costoExtra > grafo->getMaxW()) return; // se excedio el peso y no se agrega el camino encontrado

    for (int i = 1; i < (int)resto.size(); i++) {
        camino.agregarNodo(resto[i]);
    }
}

bool SolverKopt::swapValido(int idAnterior, int id, int posterior){
    return (grafo->existeArista(idAnterior, id) && grafo->existeArista(id, posterior));
}

 std::vector<std::pair<int,int>> SolverKopt::obtenerHuecos(std::vector<int> candidatos){

    std::vector<std::pair<int,int>> res;

    for (int candidato : candidatos){
        int posCandiadto = camino.getPosicionNodo(candidato);
        if (posCandiadto == -1) { return {}; }

        std::vector<int> vec = camino.getCamino();
        res.emplace_back(vec[posCandiadto - 1], vec[posCandiadto + 1]);
    }

    return res;
 }

 Camino SolverKopt::resolver(){
    construirCaminoFaltante();
    if (!camino.esCaminoCompleto()) return camino;

    bool mejoro = true;
    while (mejoro){
        mejoro = false;

        std::vector<int> cam = camino.getCamino();
        std::vector<int> intermedios(cam.begin() + 1, cam.end() - 1);

        std::vector<int> noVisitados;
        for (int i = 0; i < grafo->getCantVert(); i++){
            if (!camino.nodoFueVisitado(i)) noVisitados.push_back(i);
        }

        std::vector<std::vector<int>> combi = Algoritmo::combinar(intermedios, k);

        for (const std::vector<int>& combinacion : combi){
            std::vector<std::pair<int,int>> huecos = obtenerHuecos(combinacion);
            if (huecos.empty()) continue; 

            std::vector<std::vector<int>> permutaciones = Algoritmo::permutar(noVisitados, huecos, k, *grafo);

            for (const std::vector<int>& perm : permutaciones){
                Camino candidato = camino;
                
                for (int i = 0; i < k; i++){
                    candidato.reemplazarNodo(combinacion[i], perm[i]);
                }
                
                if (candidato.getBeneficioTotal() > camino.getBeneficioTotal() &&
                    candidato.getPesoTotal() <= grafo->getMaxW()){
                    camino = candidato;
                    mejoro = true;
                    break;
                }
            }
            if (mejoro) break;
        }
    }

    return camino;
 }

Camino SolverKopt::resolverSalto(){
    std::vector<int> cam = camino.getCamino();
    std::vector<int> intermedios(cam.begin() + 1, cam.end() - 1);

    std::vector<int> noVisitados;
    for (int i = 0; i < grafo->getCantVert(); i++){
        if (!camino.nodoFueVisitado(i)) noVisitados.push_back(i);
    }

    std::vector<std::vector<int>> combi = Algoritmo::combinar(intermedios, k);
    if (combi.empty()) return camino;

    // mezclar combinaciones para elegir al azar
    std::mt19937 rng(std::random_device{}());
    std::shuffle(combi.begin(), combi.end(), rng);

    for (const std::vector<int>& combinacion : combi){
        std::vector<std::pair<int,int>> huecos = obtenerHuecos(combinacion);
        if (huecos.empty()) continue;

        std::vector<std::vector<int>> permutaciones = Algoritmo::permutar(noVisitados, huecos, k, *grafo);
        if (permutaciones.empty()) continue;

        // elegir permutacion aleatoria
        std::uniform_int_distribution<int> dist(0, permutaciones.size() - 1);
        const std::vector<int>& perm = permutaciones[dist(rng)];

        Camino candidato = camino;
        for (int i = 0; i < k; i++){
            candidato.reemplazarNodo(combinacion[i], perm[i]);
        }

        // aplicar aunque no mejore, solo verificar que el peso sea valido
        if (candidato.getPesoTotal() <= grafo->getMaxW()){
            camino = candidato;
            return camino;
        }
    }

    return camino;
}