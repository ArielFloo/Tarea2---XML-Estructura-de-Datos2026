#ifndef ARBOL_H
#define ARBOL_H

#include "Nodo.h"
#include <iostream>

/**
 * @class ArbolLibros
 * @brief Estructura de datos de Árbol General (N-ario) para gestionar una jerarquía de libros.
 * * Esta clase implementa un árbol donde la raíz contiene una colección de libros 
 * principales, y cada libro principal puede contener múltiples libros similares 
 * como hijos directos. Proporciona métodos para recorrer, filtrar y consultar 
 * la información de los nodos por medio de la recursividad.
 */
class ArbolLibros {
private:
    NodoArbol* raiz; 

   
    void _listar_preorder(NodoArbol* nodo) {
        if (nodo == nullptr) // si el nodo no esta null retorna
            return;

        // Solo imprimimos si el nodo actual es un Libro principal
        if (nodo->tipo_nodo == "Libro") {
            std::cout << nodo->id << "\n";
        }

        for (NodoArbol* hijo : nodo->hijos) {
            _listar_preorder(hijo);
        }
    }

    
    
    // Función auxiliar para no dejar basura en la RAM, para evitar errores de crash y poder utizlizar en las otras funciones
    void _liberar_memoria(NodoArbol* nodo) {
        if (nodo == nullptr) return;
        for (NodoArbol* hijo : nodo->hijos) {
            _liberar_memoria(hijo);
        }
        delete nodo; // destruye el nodo en memoria
    }

    // Devuelve true si el nodo debe ser eliminado, false si no debe ser eliminado
    bool _borrar_ratings(NodoArbol* nodo, float r) {
        if (nodo == nullptr) return false;

        // Primero procesamos a los hijos usando iteradores del adt vector
        for (auto it = nodo->hijos.begin(); it != nodo->hijos.end(); ) {
            // Evaluamos al hijo
            bool eliminar_hijo = _borrar_ratings(*it, r);
            
            if (eliminar_hijo) {
                // Si el hijo murio, lo borramos de la lista de hijos (Vector)
                it = nodo->hijos.erase(it);
            } else {
                // Si sobrevivió avanzamos al siguiente
                ++it;
            }
        }

        // Ahora evaluamos al nodo actual
        // solo podemos borrar nodos de tipo "Libro" (No la Raiz principal ni los Similares)
        if (nodo->tipo_nodo == "Libro" && nodo->rating <= r) {
            _liberar_memoria(nodo); // Limpiamos su RAM, para esto se creo la funcion liberar memoria
            return true; // Le avisamos a su padre que lo borre de su vector
        }

        return false;
    }

    
    void _precursores(NodoArbol* nodo) {
        if (nodo == nullptr) return;

        if (nodo->tipo_nodo == "Libro") {
            bool es_precursor = true;
            bool tiene_similares_con_anio = false; // Para ignorar libros sin similares

            for (NodoArbol* hijo : nodo->hijos) {
                if (hijo->tipo_nodo == "Similar") {
                    // Validamos que tanto el padre como el hijo tengan un año válido para comparar
                    if (nodo->anio > 0 && hijo->anio > 0) {
                        tiene_similares_con_anio = true;
                        
                        // Si el hijo se publicó en el mismo año o antes, ya no es precursor
                        if (hijo->anio <= nodo->anio) {
                            es_precursor = false;
                            break; // rompemos el ciclo, no hace falta revisar más
                        }
                    }
                }
            }

            // Si sobrevivió a las validaciones, lo imprimimos
            if (tiene_similares_con_anio && es_precursor) {
                std::cout << nodo->id << "\n";
            }
        }

        // Seguimos recorriendo el árbol
        for (NodoArbol* hijo : nodo->hijos) {
            _precursores(hijo);
        }
    }

public:
    ArbolLibros() {
        raiz = new NodoArbol("Raiz"); // aqui iniciamos un nuevo nodo con el constructor
    }

    void agregar_libro_principal(NodoArbol* nuevo_libro) {
        raiz->hijos.push_back(nuevo_libro);
    }
    
    NodoArbol* get_raiz() {
        return raiz;
    }
   
    /**
     * @brief Lista los IDs de los libros principales del árbol.
     * * Realiza un recorrido de tipo Preorder (Preorden) sobre la estructura del árbol.
     * Durante el recorrido, evalúa el tipo de nodo y, si corresponde a un "Libro" 
     * principal, imprime el ID correspondiente como salida.
     */
    void listar() {
        std::cout << "--- Listando IDs (Preorder) ---" << std::endl;
        _listar_preorder(raiz); 
    }
    /**
     * @brief Elimina del árbol todos los libros con un rating menor o igual a 'r'.
     * * @param r Umbral de calificación. Los libros con rating <= r serán eliminados.
     * * Recorre el árbol evaluando las calificaciones. Utiliza iteradores sobre 
     * el vector de hijos para eliminar las referencias sin romper la estructura general. 
     * Adicionalmente, libera la memoria dinámica (subárboles) de los nodos a medida que se van descartando.
     */
    void borrar_ratings(float r) {
        std::cout << "--- Borrando libros con rating <= " << r << " ---" << std::endl;
        _borrar_ratings(raiz, r);
        std::cout << "Borrado completado." << std::endl;
    }
    /**
     * @brief Identifica y lista los IDs de los libros considerados "precursores".
     * * Un libro se considera precursor si posee libros similares (hijos) y el año de 
     * publicación de TODOS sus libros similares es estrictamente mayor (posterior) 
     * al año de publicación del libro principal. Esta sección imprime los IDs que cumplen dicha condición.
     */
    void precursores() {
        std::cout << "--- Listando IDs de Precursores ---" << std::endl;
        _precursores(raiz);
    }
};

#endif