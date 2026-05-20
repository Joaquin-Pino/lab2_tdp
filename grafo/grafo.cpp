#include "grafo.h"
#include <fstream>
#include <stdexcept>
#include <queue>
#include <sstream>
#include <climits>

Grafo::Grafo(int cantVert, int cantArist, int maxW)
    : cantVert(cantVert), cantArist(cantArist), maxW(maxW),
      listaAdy(cantVert) {}

Grafo Grafo::cargarDesdeArchivo(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);

    int n, m, W;
    file >> n >> m >> W;

    Grafo g(n, m, W);

    int origen, destino, costo, beneficio;
    for (int k = 0; k < m; k++) {
        file >> origen >> destino >> costo >> beneficio;
        g.insertarArista(origen, destino, costo, beneficio);
    }
    return g;
}

void Grafo::insertarArista(int origen, int destino, int costo, int beneficio) {
    listaAdy[origen].push_back({destino, costo, beneficio});
}

std::vector<Nodo> Grafo::getVecinos(int idNodo) const {
    return listaAdy[idNodo];
}

int Grafo::getCantVert() const { return cantVert;}
int Grafo::getMaxW() const { return maxW;}
int Grafo::getNodoDestino() const { return cantVert - 1;}

std::vector<int> Grafo::dijkstra(int origen) const {
    std::vector<int> dist(cantVert, INT_MAX);
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    dist[origen] = 0;
    pq.push({0, origen});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (const Nodo& v : listaAdy[u]) {
            if (dist[u] + v.costo < dist[v.destino]) {
                dist[v.destino] = dist[u] + v.costo;
                pq.push({dist[v.destino], v.destino});
            }
        }
    }
    return dist;
}

std::vector<int> Grafo::dijkstraInvertido(int destino) const {
    // Construye lista de adyacencia invertida y corre Dijkstra desde destino.
    // El resultado dist[v] = costo mínimo en peso de v hasta destino.
    std::vector<std::vector<Nodo>> listaInv(cantVert);
    for (int u = 0; u < cantVert; u++) {
        for (const Nodo& v : listaAdy[u]) {
            listaInv[v.destino].push_back({u, v.costo, v.beneficio});
        }
    }

    std::vector<int> dist(cantVert, INT_MAX);
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    dist[destino] = 0;
    pq.push({0, destino});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (const Nodo& v : listaInv[u]) {
            if (dist[u] + v.costo < dist[v.destino]) {
                dist[v.destino] = dist[u] + v.costo;
                pq.push({dist[v.destino], v.destino});
            }
        }
    }
    return dist;
}


Nodo Grafo::getArista(int a, int b) const{
    // retorna {costo, beneficio}, {0,0} en caso de que no exista arista
    std::vector<Nodo> vecinos = getVecinos(a);
    Nodo nodo;
    for (const Nodo& n : vecinos){
        if (n.destino == b){
            nodo = n;
            return nodo;
        }
    }

    // tiramos error ya que no se pueden devolver referencias nulas
    std::ostringstream msg;
    msg << "no se encontro arista desde " << a << " hasta "<< b;
    throw std::runtime_error(msg.str());
}