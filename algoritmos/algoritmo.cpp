#include "algoritmo.h"

std::vector<std::vector<int>> Algoritmo::combinar(const std::vector<int>& candidatos, int k) {
    
    std::vector<std::vector<int>> res;
    std::vector<int> acum;
    if (k > (int)candidatos.size() || k < 0) return res;
    combinarAux(candidatos, k, 0, acum, res);

    return res;

}
void Algoritmo::combinarAux(const std::vector<int>& candidatos, int k, int i, std::vector<int>& acum, std::vector<std::vector<int>>& resultado){
    if ((int) acum.size() == k){
        resultado.push_back(acum);
        return;
    }

    if (i >= (int)candidatos.size()) {
        return;
    }

    acum.push_back(candidatos[i]);
    combinarAux(candidatos, k, i+1, acum, resultado);
    acum.pop_back();

    //no elegimos el elemento
    combinarAux(candidatos, k, i + 1, acum, resultado);
}

// se incluyen los huecos generados para hacer podas y no generar todas las permutaciones posibles
std::vector<std::vector<int>> Algoritmo::permutar(const std::vector<int>& candidatos, const std::vector<std::pair<int,int>>& huecos, int k, const Grafo& grafo) {
    std::vector<std::vector<int>> res;
    std::vector<int> acum;
    std::vector<bool> usados(candidatos.size(), false);

    permutarAux(candidatos, huecos, k, 0, usados, acum, res, grafo);

    return res;
}

void Algoritmo::permutarAux(const std::vector<int>& candidatos, const std::vector<std::pair<int,int>>& huecos, 
                int k, int j, std::vector<bool>& usados, std::vector<int>& acum, 
                std::vector<std::vector<int>>& resultado, const Grafo& grafo){

    if((int) acum.size() == k){
        resultado.push_back(acum);
        return;
    }

    for (int i = 0; i < (int)candidatos.size(); ++i) {
        
        // Control básico de permutación: no usar el mismo elemento dos veces
        if (usados[i]) continue; 

        int in_j = candidatos[i];

        //  Verificación de la poda con el hueco j
        // Hueco j tiene restricciones: huecos[j].first -> in_j -> huecos[j].second
        int origen = huecos[j].first;
        int destino = huecos[j].second;

        // Si alguna de las aristas requeridas no existe, saltamos este candidato
        if (!grafo.existeArista(origen, in_j) || !grafo.existeArista(in_j, destino)) {
            continue;
        }

        acum.push_back(in_j);
        usados[i] = true; // Marcar como usado

        // Avanzamos al siguiente hueco (j + 1)
        permutarAux(candidatos, huecos, k, j + 1, usados, acum, resultado, grafo);

        usados[i] = false;
        acum.pop_back();  
    }

    

}
