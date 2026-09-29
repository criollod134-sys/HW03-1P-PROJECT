#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream> // Pongo esta libreria para poder leer y escribir archivos txt

using namespace std;

// ======================================================
// 1. ESTRUCTURA BÁSICA (NODO)
// ======================================================
// Estructura para cada carpeta o archivo
struct NodoArchivo {
    string nombre;
    bool esArchivo; // true si es archivo (1), false si es carpeta (0)
    
    // Uso punteros para conectar todo. 
    // Uno apunta a lo que hay adentro de la carpeta y otro a lo que esta al lado.
    NodoArchivo* primerHijo;       
    NodoArchivo* siguienteHermano; 
};

// Funcion para crear un nodo nuevo en blanco
NodoArchivo* crearNodo(string nombre, bool esArchivo) {
    NodoArchivo* nuevo = new NodoArchivo; // Pido memoria con new
    nuevo->nombre = nombre;
    nuevo->esArchivo = esArchivo;
    nuevo->primerHijo = NULL;
    nuevo->siguienteHermano = NULL;
    return nuevo;
}

// Funcion para meter un elemento dentro de una carpeta
void agregarElemento(NodoArchivo* carpetaPadre, NodoArchivo* nuevoElemento) {
    // Si la carpeta esta vacia, entra directo como el primer hijo
    if (carpetaPadre->primerHijo == NULL) {
        carpetaPadre->primerHijo = nuevoElemento;
    } else {
        // Si ya hay cosas, recorro la fila de hermanos hasta llegar al ultimo
        NodoArchivo* temporal = carpetaPadre->primerHijo;
        while (temporal->siguienteHermano != NULL) {
            temporal = temporal->siguienteHermano;
        }
        // Lo engancho al final de la fila
        temporal->siguienteHermano = nuevoElemento;
    }
}

// ======================================================
// 2. CONSTRUCTOR DINÁMICO 
// ======================================================
// Esta funcion lee el arbol.txt para no poner los datos fijos en el codigo
NodoArchivo* cargarArbolDesdeTXT(string nombreArchivo) {
    ifstream archivo(nombreArchivo); // Abro el archivo de texto
    if (!archivo.is_open()) {
        cout << "ERROR: No se encontro el archivo " << nombreArchivo << "\n";
        return NULL;
    }

    string padre, hijo;
    int esArchivo;
    
    NodoArchivo* raiz = NULL;
    // Uso un vector para ir guardando temporalmente los nodos que voy creando
    vector<NodoArchivo*> todosLosNodos; 

    // Voy leyendo palabra por palabra del txt: Padre -> Hijo -> Tipo (0 o 1)
    while (archivo >> padre >> hijo >> esArchivo) {
        
        NodoArchivo* nodoPadre = NULL;
        
        // Busco en mi vector si la carpeta padre ya la habia creado antes
        for (int i = 0; i < todosLosNodos.size(); i++) {
            if (todosLosNodos[i]->nombre == padre) {
                nodoPadre = todosLosNodos[i];
                break;
            }
        }
        
        // Si no existe, la creo (pongo false porque los padres siempre son carpetas)
        if (nodoPadre == NULL) {
            nodoPadre = crearNodo(padre, false);
            todosLosNodos.push_back(nodoPadre);
            
            // El primer padre que aparece en el txt sera la Raiz (por ejemplo C:)
            if (raiz == NULL) {
                raiz = nodoPadre;
            }
        }

        // Creo el hijo. Si esArchivo es 1, tipoArchivo es true.
        bool tipoArchivo = (esArchivo == 1);
        NodoArchivo* nodoHijo = crearNodo(hijo, tipoArchivo);
        
        // Los conecto
        agregarElemento(nodoPadre, nodoHijo);
        
        // Lo guardo en la lista general
        todosLosNodos.push_back(nodoHijo);
    }

    archivo.close();
    return raiz; // Devuelvo el arbol ya armado
}

// ======================================================
// 3. ALGORITMOS DE BÚSQUEDA Y REGISTRO
// ======================================================

// Busqueda DFS (Profundidad). Usa recursividad (la funcion se llama a si misma)
bool buscarDFS(NodoArchivo* nodo, string objetivo, string rutaActual, vector<string>& historial) {
    if (nodo == NULL) return false; // Si me topo con vacio, regreso falso
    
    // Voy armando la ruta y la guardo en el vector
    rutaActual = rutaActual + "/" + nodo->nombre;
    historial.push_back(rutaActual); 
    
    // Reviso si ya encontre el archivo 
    if (nodo->nombre == objetivo) {
        return true; 
    }
    
    // Si no es, bajo a los hijos y sigo buscando hasta el fondo
    NodoArchivo* hijo = nodo->primerHijo;
    while (hijo != NULL) {
        if (buscarDFS(hijo, objetivo, rutaActual, historial) == true) {
            return true;
        }
        hijo = hijo->siguienteHermano; // Si no esta en un hijo, paso al hermano de al lado
    }
    
    return false; 
}

// Busqueda BFS (Anchura). Avanza por niveles usando colas.
void buscarBFS(NodoArchivo* raiz, string objetivo, vector<string>& historial) {
    if (raiz == NULL) return;
    
    // Hago dos colas: una para los nodos y otra para ir guardando la ruta de texto
    queue<NodoArchivo*> colaNodos;
    queue<string> colaRutas;
    
    // Meto la raiz primero a las colas
    colaNodos.push(raiz);
    colaRutas.push(raiz->nombre);
    
    // Mientras la cola tenga elementos, sigo revisando
    while (colaNodos.empty() == false) {
        // Saco el primero de la fila
        NodoArchivo* actual = colaNodos.front();
        string rutaActual = colaRutas.front();
        
        colaNodos.pop();
        colaRutas.pop();
        
        // Anoto por donde pase en el historial
        historial.push_back(rutaActual); 
        
        // Si ya lo encontre, termino la busqueda con return
        if (actual->nombre == objetivo) {
            return; 
        }
        
        // Si no lo encontre, meto a todos sus hijos a la cola para revisarlos despues
        NodoArchivo* hijo = actual->primerHijo;
        while (hijo != NULL) {
            colaNodos.push(hijo);
            colaRutas.push(rutaActual + "/" + hijo->nombre);
            hijo = hijo->siguienteHermano;
        }
    }
}

// ======================================================
// 4. FUNCIÓN PARA GUARDAR RESULTADOS
// ======================================================
// Esto es para guardar lo que hicieron los vectores en un archivo de texto
void guardarEnTXT(const vector<string>& historialDFS, const vector<string>& historialBFS) {
    ofstream archivo("registro_busqueda.txt"); // Creo el archivo
    
    if (archivo.is_open()) {
        archivo << "====================================================\n";
        archivo << " REPORTE DE EXPLORACION - MAQUINA DE ESTADO FINITO\n";
        archivo << "====================================================\n\n";
        
        archivo << "--- RESULTADOS BUSQUEDA EN PROFUNDIDAD (DFS) ---\n";
        archivo << "Total de nodos visitados: " << historialDFS.size() << "\n";
        for (size_t i = 0; i < historialDFS.size(); i++) {
            archivo << "Paso " << i + 1 << ": " << historialDFS[i] << "\n";
        }
        
        archivo << "\n----------------------------------------------------\n\n";
        
        archivo << "--- RESULTADOS BUSQUEDA EN ANCHURA (BFS) ---\n";
        archivo << "Total de nodos visitados: " << historialBFS.size() << "\n";
        for (size_t i = 0; i < historialBFS.size(); i++) {
            archivo << "Paso " << i + 1 << ": " << historialBFS[i] << "\n";
        }
        
        archivo.close();
        cout << "\n[EXITO] El registro se guardo en el archivo 'registro_busqueda.txt'\n";
    } else {
        cout << "\n[ERROR] No se pudo crear el reporte.\n";
    }
}

// ======================================================
// 5. MAIN 
// ======================================================
int main() {
    cout << "======================================\n";
    cout << " SISTEMA DE BUSQUEDA JERARQUICO\n";
    cout << "======================================\n\n";

    // Llamo a la funcion para leer el arbol desde el txt
    NodoArchivo* raiz = cargarArbolDesdeTXT("arbol.txt");
    
    // Si no encontro el archivo txt, cierro el programa con return 1
    if (raiz == NULL) {
        return 1; 
    }

    // Le pido al usuario que escriba el archivo usando cin
    string objetivo;
    cout << "Escribe el nombre exacto del archivo que deseas buscar: ";
    cin >> objetivo;
    cout << "\n";
    
    // Vectores para ir guardando la ruta paso a paso
    vector<string> historialDFS;
    vector<string> historialBFS;
    
    // Corro la busqueda DFS
    cout << "Buscando '" << objetivo << "' con DFS...\n";
    bool halladoDFS = buscarDFS(raiz, objetivo, "", historialDFS);
    if (halladoDFS) {
        cout << "  -> Archivo encontrado. Nodos visitados: " << historialDFS.size() << "\n\n";
    } else {
        cout << "  -> Archivo NO encontrado. Nodos visitados: " << historialDFS.size() << "\n\n";
    }
    
    // Corro la busqueda BFS
    cout << "Buscando '" << objetivo << "' con BFS...\n";
    buscarBFS(raiz, objetivo, historialBFS);
    
    // Reviso si el ultimo lugar de la cola del BFS era mi archivo objetivo usando find
    if (!historialBFS.empty() && historialBFS.back().find(objetivo) != string::npos) {
        cout << "  -> Archivo encontrado. Nodos visitados: " << historialBFS.size() << "\n";
    } else {
        cout << "  -> Archivo NO encontrado. Nodos visitados: " << historialBFS.size() << "\n";
    }
    
    // Llamo a la funcion para generar el bloc de notas con los resultados
    guardarEnTXT(historialDFS, historialBFS);
    
    return 0;
}