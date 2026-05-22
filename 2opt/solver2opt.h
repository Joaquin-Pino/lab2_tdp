#pragma once
#include <vector>
#include "../camino/camino.h"
#include "../grafo/grafo.h"

struct CandidatoNodo {
    int id;
    float ratio;
    bool operator<(const CandidatoNodo& otro) const { return ratio < otro.ratio; }
};

class Solver2opt {
private:
    Camino camino;
    const Grafo* grafo;

    void construirCaminoFaltante();

public:
    Solver2opt();
    Solver2opt(const Grafo& grafo, Camino c);

    Camino resolver();
};
