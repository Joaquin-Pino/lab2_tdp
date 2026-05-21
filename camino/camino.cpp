#include "camino.h"

Camino::Camino() : pesoTotal(0), beneficioTotal(0), grafo(nullptr) {}

Camino::Camino(std::vector<int> camino, const Grafo& grafo) :  pesoTotal(0), beneficioTotal(0), camino(camino){
    this->grafo = &grafo;
    calcularYAsignarPesoYBeneficio();
}

void Camino::agregarNodo(int id){
    if (visitados.count(id)) return;
    visitados.insert(id);

    if (!camino.empty()){
        int idUltimoNodo = camino.back();
        for (const Nodo& n : grafo->getVecinos(idUltimoNodo)){
            if (n.destino == id){
                pesoTotal += n.costo;
                beneficioTotal += n.beneficio;
                break;
            }
        }
    }

    camino.push_back(id);
}

void Camino::eliminarNodo(int id){
    // borrar del camino
    for (int i = 0; i < camino.size() ; i++){
        if (camino[i] == id){
            Nodo temp = grafo->getArista(camino[i - 1], id);
            Nodo temp2 = grafo->getArista(id, camino[i+1]);

            // reducir pesoTotal y beneficioTotal
            pesoTotal -= (temp.costo + temp2.costo);
            beneficioTotal -= (temp.beneficio + temp2.beneficio);

            camino.erase(camino.begin() + i); 
            break;
        }
    }
    // borrar de visitado
    visitados.erase(id);
}

void Camino::intercambiarNodos(int id1, int id2){
    int indx1 = -1;
    int indx2 = -1;

    for (int i = 0; i < camino.size(); i++){
        if (id1 == camino[i]){
            indx1 = i;
        }
        if (id2 == camino[i]){
            indx2 = i;
        }
    }

    if (indx1 == -1 || indx2 == -1){
        throw std::runtime_error("no se encontro un nodo en el camino para hacer intercambio");
    }
    
    int temp = camino[indx1];
    camino[indx1] = camino[indx2];
    camino[indx2] = temp;

    calcularYAsignarPesoYBeneficio();
}

void Camino::calcularYAsignarPesoYBeneficio(){
    if (camino.size() == 0){
        return;
    }

    int costo = 0;
    int beneficio = 0;
    for (int i = 1; i < camino.size(); i++){
        Nodo temp = grafo->getArista(camino[i -1], camino[i]);
        costo += temp.costo;
        beneficio += temp.beneficio;
    }

    pesoTotal = costo;
    beneficioTotal = beneficio;
}

void Camino::marcarNodoVisitado(int id){
    if (visitados.find(id) != visitados.end()){
        return;
    }
    visitados.insert(id);
}

bool Camino::nodoFueVisitado(int id) const {
    return visitados.find(id) != visitados.end();
}

bool Camino::verificarCamino(int wMax){
    return pesoTotal <= wMax;
}

int Camino::getUltimoNodo(){
    if (camino.size() == 0){
        throw std::runtime_error("no se puede obtener el ultimo nodo de un camino vacio");
    }

    return camino[camino.size() -1];
}

std::vector<int> Camino::getCamino(){
    return camino;
}

int Camino::getBeneficioTotal(){
    return beneficioTotal;
}

int Camino::getPesoTotal(){
    return pesoTotal;
}