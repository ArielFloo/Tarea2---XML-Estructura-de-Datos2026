#ifndef NODO_H
#define NODO_H

#include <string>
#include <vector>

// Esta estructura representa cualquier nodo de nuestro árbol
struct NodoArbol {
    std::string tipo_nodo; // Para saber si es la Raiz, un Libro o un Similar, que son las tres posibilidades que manejamos que nos pide la tarea
    
    // Los datos que sacamos del archivo
    std::string id;
    std::string titulo;
    std::string isbn;
    int anio;
    std::string idioma;
    float rating;
    int paginas;

    // Un vector que guarda punteros hacia sus hijos
    std::vector<NodoArbol*> hijos;

    // Constructor básico para crear nodos rápidamente
    NodoArbol(std::string tipo) {
        tipo_nodo = tipo;
        id = "";
        titulo = "";
        isbn = "";
        anio = 0;
        idioma = "";
        rating = 0.0f;
        paginas = 0;
    }
};

#endif