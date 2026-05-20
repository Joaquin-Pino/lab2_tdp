#include "camino.h"


Camino::Camino() : pesoTotal(0), beneficioTotal(0) {}

Camino::Camino(std::vector<int> camino, const Grafo& grafo) : camino(camino){
    pesoTotal = calcularPesoTotal(grafo);
    beneficioTotal = calcularBeneficioTotal(grafo);
    this->grafo = &grafo;
}

void Camino::agregarNodo(int id){
    // nodo no debe estar visitado
    if (visitados.count(id)){
        return; // nodo ya se encuentra, no hacer nada
    }
    // caso camino vacio
    if(camino.size() == 0){
        visitados.insert(id);
        camino.push_back(id);

         std::vector<Nodo> vecinos = grafo->getVecinos(0); // primer nodo
        
        //  TODO: busqueda debe ser log(n)
         for (const Nodo& n : vecinos){
            if (n.destino == id){
                pesoTotal += n.costo;
                beneficioTotal += n.beneficio;
                break;
            }
         }
        return;
    }
    // agregamos nodo
    visitados.insert(id);
    int idUltimoNodo = camino.at(camino.size() - 1);
    // sumar costo

    std::vector<Nodo> vecinos = grafo->getVecinos(idUltimoNodo);

    // TODO ESTA BUSQUEDA DEBERIA SER EN O(logN)
    for(const Nodo& n : vecinos){
        if (n.destino == id){
            pesoTotal += n.costo;
            beneficioTotal += n.beneficio;
            break;
        }
    }

    camino.push_back(id);
}

void Camino::eliminarNodo(int id){
    std::vector<Nodo> vecinos = grafo->getVecinos(id);

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

    recalcularPesoYBeneficio();
    // calcular nuevo peso y beneficio del camino
}

void Camino::recalcularPesoYBeneficio(){
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