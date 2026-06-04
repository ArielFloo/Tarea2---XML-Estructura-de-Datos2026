#include <iostream>
#include <vector>
#include <string>

// Estructura genérica para representar la información de un libro
struct Libro {
    std::string id;
    std::string titulo;
    std::string isbn;
    int anio_publicacion;
    std::string idioma;
    std::string descripcion;
    double rating;
    int paginas;
    // Nota: Para los libros similares dentro del XML, puedes estructurarlos 
    // como hijos directos o como una lista interna aquí mismo.
};

struct Node {
    Libro info;
    std::vector<Node*> hijos; // Lista de hijos para un árbol general

    Node(Libro libro) : info(libro) {}
};

void listar(Node* raiz) {
    // Caso extremo: Si el nodo es nulo, no hay nada que listar
    if (raiz == nullptr) return;
    
    // 1. Procesar el nodo actual (imprimir su ID)
    std::cout << "ID: " << raiz->info.id << "\n";
    for (Node* hijo : raiz->hijos) {
        listar(hijo);
    }
}

// Función auxiliar para liberar la memoria de todo un subárbol
void liberar_arbol(Node* raiz) {
    if (raiz == nullptr) return;
    for (Node* hijo : raiz->hijos) {
        liberar_arbol(hijo);
    }
    delete raiz;
}

// Función principal para borrar nodos con rating menor o igual a r
Node* borrar_ratings(Node* raiz, double r) {
    if (raiz == nullptr) return nullptr;

    for (auto it = raiz->hijos.begin(); it != raiz->hijos.end(); ) {
        // Llamada recursiva al hijo
        Node* hijo_procesado = borrar_ratings(*it, r);
        
        if (hijo_procesado == nullptr) {
            // Si la función devolvió nullptr, significa que el hijo se eliminó.
            // Lo sacamos de la lista de hijos del nodo actual.
            it = raiz->hijos.erase(it);
        } else {
            *it = hijo_procesado;
            ++it;
        }
    }

    // Verificación del nodo actual después de procesar sus hijos
    if (raiz->info.rating <= r) {
        liberar_arbol(raiz); 
        return nullptr;
    }
    return raiz;
}

// Función auxiliar precursividad
bool es_precursor(Node* nodo) {
    // Si no hay libros similares, no cumple la condición
    if (nodo == nullptr || nodo->hijos.empty()) {
        return false;
    }
    
    for (Node* hijo : nodo->hijos) {
        if (hijo->info.anio_publicacion <= nodo->info.anio_publicacion) {
            return false;
        }
    }
    return true;
}
