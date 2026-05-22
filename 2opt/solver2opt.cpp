#include "solver2opt.h"
#include <algorithm>
Solver2opt::Solver2opt() : grafo(nullptr) {}

Solver2opt::Solver2opt(const Grafo& grafo, Camino c) : camino(c), grafo(&grafo) {}

void Solver2opt::construirCaminoFaltante() {
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
        } catch (...) { return; }
    }

    if (camino.getPesoTotal() + costoExtra > grafo->getMaxW()) return;

    for (int i = 1; i < (int)resto.size(); i++)
        camino.agregarNodo(resto[i]);
}

Camino Solver2opt::resolver() {
    construirCaminoFaltante();
    if (!camino.esCaminoCompleto()) return camino;

    bool mejoro = true;
    while (mejoro) {
        mejoro = false;

        std::vector<CandidatoNodo> sacar, meter;
        std::vector<int> cam = camino.getCamino();

        // nodos intermedios del camino, ordenados por ratio ASC (peores primero)
        for (int i = 1; i < (int)cam.size() - 1; i++)
            sacar.push_back({cam[i], camino.getRatioNodo(cam[i])});
        std::sort(sacar.begin(), sacar.end());

        // nodos fuera del camino, ordenados por ratio DESC (mejores primero)
        for (int id = 0; id < grafo->getCantVert(); id++) {
            if (camino.nodoFueVisitado(id)) continue;
            meter.push_back({id, grafo->getRatioMejorEntrada(id)});
        }
        std::sort(meter.begin(), meter.end(), [](const CandidatoNodo& a, const CandidatoNodo& b){ return a.ratio > b.ratio; });

        for (const CandidatoNodo& out : sacar) {
            cam = camino.getCamino();
            int pos = -1;
            for (int j = 1; j < (int)cam.size() - 1; j++) {
                if (cam[j] == out.id) { pos = j; break; }
            }
            if (pos == -1) continue;
            int prev = cam[pos - 1], next = cam[pos + 1];

            for (const CandidatoNodo& in_ : meter) {
                try {
                    Nodo entra = grafo->getArista(prev, in_.id);
                    Nodo sale = grafo->getArista(in_.id, next);
                    Nodo entra_old = grafo->getArista(prev, out.id);
                    Nodo sale_old  = grafo->getArista(out.id, next);

                    int delta_peso = (entra.costo + sale.costo) - (entra_old.costo + sale_old.costo);
                    int delta_ben  = (entra.beneficio + sale.beneficio) - (entra_old.beneficio + sale_old.beneficio);

                    if (camino.getPesoTotal() + delta_peso <= grafo->getMaxW() && delta_ben > 0) {
                        camino.reemplazarNodo(out.id, in_.id);
                        mejoro = true;
                        break;
                    }
                } catch (...) {
                    continue; // arista no existe, probar siguiente
                }
            }
            if (mejoro) break;
        }
    }
    return camino;
}
