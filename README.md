# Proyecto: Árbol General con XML de Goodreads

## Estrategia de Procesamiento (XML a C++)
Para procesar los 10.000 archivos XML, utilizamos un script en Python (`limpiar_datos.py`) que extrae únicamente la información solicitada y genera un archivo plano llamado `libros_limpios.csv`. 
Para facilitar la revisión, hemos incluido el archivo `libros_limpios.csv` en este repositorio, de modo que el código en C++ pueda ser compilado y ejecutado directamente sin necesidad de volver a procesar el dataset.



## Instrucciones de Compilación y Ejecución (C++)
Para compilar el proyecto utilizando el compilador g++, abra una terminal en el directorio raíz del proyecto y ejecute el siguiente comando:

> g++ -o programa main.cpp

Una vez compilado sin errores, ejecute el programa con el siguiente comando:

En Windows:
> .\programa.exe

En Linux/Mac:
> ./programa