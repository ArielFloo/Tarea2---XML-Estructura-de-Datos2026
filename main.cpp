#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Arbol.h"

using namespace std;

// Función auxiliar: Corta un texto en pedazos
vector<string> separar_texto(const string& texto, char delimitador) {
    vector<string> pedazos;
    string pedazo;
    istringstream tokenStream(texto);
    while (getline(tokenStream, pedazo, delimitador)) {
        pedazos.push_back(pedazo);
    }
    return pedazos;
}


int safe_stoi(const string& str) {
    if (str.empty()) return 0;
    try { 
        return stoi(str); 
    } catch (...) { 
        // Si no es un número o tiene basura, retornamos 0 en silencio
        return 0; 
    }
}

float safe_stof(const string& str) {
    if (str.empty()) return 0.0f;
    try { 
        return stof(str); 
    } catch (...) { 
        return 0.0f; 
    }
}


int main() {
    ArbolLibros miArbol;
    
    ifstream archivo("libros_limpios.csv");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir 'libros_limpios.csv'." << endl;
        return 1;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezados

    int contador_libros = 0;

    while (getline(archivo, linea)) {
        vector<string> datos = separar_texto(linea, '|');
        if (datos.size() < 8) continue; 

        NodoArbol* nuevo_libro = new NodoArbol("Libro");
        nuevo_libro->id = datos[0];
        nuevo_libro->titulo = datos[1];
        nuevo_libro->isbn = datos[2];
        
        // APLICAMOS NUESTROS ESCUDOS AQUÍ
        nuevo_libro->anio = safe_stoi(datos[3]);
        nuevo_libro->idioma = datos[4];
        nuevo_libro->rating = safe_stof(datos[5]);
        nuevo_libro->paginas = safe_stoi(datos[6]);

        string similares_str = datos[7];
        if (!similares_str.empty()) {
            vector<string> lista_similares = separar_texto(similares_str, ';');
            for (const string& sim : lista_similares) {
                if (sim.empty()) continue;
                
                vector<string> datos_sim = separar_texto(sim, '@');
                if (datos_sim.size() == 3) {
                    NodoArbol* nodo_sim = new NodoArbol("Similar");
                    nodo_sim->titulo = datos_sim[0];
                    nodo_sim->isbn = datos_sim[1];
                    
                    // ESCUDO APLICADO A LOS NIETOS
                    nodo_sim->anio = safe_stoi(datos_sim[2]);
                    
                    nuevo_libro->hijos.push_back(nodo_sim);
                }
            }
        }

        miArbol.agregar_libro_principal(nuevo_libro);
        contador_libros++;
    }

    archivo.close();

    cout << "========================================" << endl;
    cout << "========================================" << endl;
    cout << "¡Arbol construido exitosamente en RAM" << endl;
    cout << "Total de libros cargados en el nivel 1: " << contador_libros << endl;
    
    if (contador_libros > 0) {
        NodoArbol* primer_libro = miArbol.get_raiz()->hijos[0];
        cout << "\nEl primer libro cargado es: " << primer_libro->titulo << endl;
        cout << "Este libro tiene " << primer_libro->hijos.size() << " libros similares (nietos)." << endl;
        
        if (!primer_libro->hijos.empty()) {
            cout << "Uno de esos similares es: " << primer_libro->hijos[0]->titulo << endl;
        }
    }
    cout << "========================================" << endl;
    cout << "========================================" << endl;


    cout << "\n1. Probando Listar:" << endl;
    miArbol.listar(); // Descomenta para probar

    cout << "\n2. Buscando Precursores:" << endl;
    miArbol.precursores();

    cout << "\n3. Borrando libros malos (rating <= 3.5)" << endl;
    int libros_antes = miArbol.get_raiz()->hijos.size();
    miArbol.borrar_ratings(3.5f);
    int libros_despues = miArbol.get_raiz()->hijos.size();
    
    cout << "Libros originales en Nivel 1: " << libros_antes << endl;
    cout << "Libros sobrevivientes en Nivel 1: " << libros_despues << endl;
    return 0;
}

