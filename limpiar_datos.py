import os
import xml.etree.ElementTree as ET
import csv

carpeta_xml = './books_xml'
archivo_salida = 'libros_limpios.csv'

with open(archivo_salida, mode='w', newline='', encoding='utf-8') as archivo_csv:
    escritor = csv.writer(archivo_csv, delimiter='|')
    escritor.writerow(['ID', 'Titulo', 'ISBN', 'Anio', 'Idioma', 'Rating', 'Paginas', 'Similares'])

    for nombre_archivo in os.listdir(carpeta_xml):
        if nombre_archivo.endswith('.xml'):
            ruta_completa = os.path.join(carpeta_xml, nombre_archivo)
            
            try:
                arbol = ET.parse(ruta_completa)
                raiz = arbol.getroot() # Esto entra a <GoodreadsResponse>
                
                # ¡LA CLAVE ESTÁ AQUÍ! Primero entramos a la etiqueta <book>
                libro = raiz.find('book')
                
                if libro is not None:
                    # Ahora extraemos todo desde 'libro'
                    book_id = libro.find('id').text if libro.find('id') is not None else ""
                    titulo = libro.find('title').text if libro.find('title') is not None else ""
                    isbn = libro.find('isbn').text if libro.find('isbn') is not None else ""
                    anio = libro.find('publication_year').text if libro.find('publication_year') is not None else ""
                    
                    # Manejamos el idioma (Goodreads suele usar language_code)
                    idioma_node = libro.find('language_code')
                    if idioma_node is None: idioma_node = libro.find('language')
                    idioma = idioma_node.text if idioma_node is not None else ""
                    
                    rating = libro.find('average_rating').text if libro.find('average_rating') is not None else "0"
                    paginas = libro.find('num_pages').text if libro.find('num_pages') is not None else "0"
                    
                    # Extraemos los libros similares
                    similares_str = ""
                    etiqueta_similares = libro.find('similar_books')
                    if etiqueta_similares is not None:
                        for libro_sim in etiqueta_similares.findall('book'):
                            sim_titulo = libro_sim.find('title').text if libro_sim.find('title') is not None else ""
                            sim_isbn = libro_sim.find('isbn').text if libro_sim.find('isbn') is not None else ""
                            sim_anio = libro_sim.find('publication_year').text if libro_sim.find('publication_year') is not None else ""
                            similares_str += f"{sim_titulo}@{sim_isbn}@{sim_anio};"
                    
                    # Escribimos la fila en el CSV
                    escritor.writerow([book_id, titulo, isbn, anio, idioma, rating, paginas, similares_str])
                    
            except Exception as e:
                print(f"Error procesando {nombre_archivo}: {e}")

print("¡Extracción terminada! Revisa el archivo", archivo_salida)