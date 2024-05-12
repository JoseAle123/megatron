#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h> // Para mkdir

using namespace std;

// Definición de la clase Sector
class Sector {
public:
    Sector() {}
};

// Definición de la clase Pista
class Pista {
private:
    int numSectores;
    Sector* sectores;

public:
    // Constructor predeterminado
    Pista() : numSectores(0), sectores(nullptr) {}

    // Constructor con parámetros
    Pista(int sectores) : numSectores(sectores) {
        this->sectores = new Sector[numSectores];
    }

    // Constructor de copia
    Pista(const Pista& otro) : numSectores(otro.numSectores) {
        sectores = new Sector[numSectores];
        for (int i = 0; i < numSectores; ++i) {
            sectores[i] = otro.sectores[i];
        }
    }

    // Operador de asignación
    Pista& operator=(const Pista& otro) {
        if (this != &otro) {
            delete[] sectores;
            numSectores = otro.numSectores;
            sectores = new Sector[numSectores];
            for (int i = 0; i < numSectores; ++i) {
                sectores[i] = otro.sectores[i];
            }
        }
        return *this;
    }

    ~Pista() {
        delete[] sectores;
    }

    int getNumSectores() const {
        return numSectores;
    }
};

// Definición de la clase Superficie
class Superficie {
private:
    int numPistas;
    Pista* pistas;

public:
    // Constructor predeterminado
    Superficie() : numPistas(0), pistas(nullptr) {}

    // Constructor con parámetros
    Superficie(int pistas, int sectores) : numPistas(pistas) {
        this->pistas = new Pista[numPistas];
        for (int i = 0; i < numPistas; ++i) {
            this->pistas[i] = Pista(sectores);
        }
    }

    // Constructor de copia
    Superficie(const Superficie& otro) : numPistas(otro.numPistas) {
        pistas = new Pista[numPistas];
        for (int i = 0; i < numPistas; ++i) {
            pistas[i] = otro.pistas[i];
        }
    }

    // Operador de asignación
    Superficie& operator=(const Superficie& otro) {
        if (this != &otro) {
            delete[] pistas;
            numPistas = otro.numPistas;
            pistas = new Pista[numPistas];
            for (int i = 0; i < numPistas; ++i) {
                pistas[i] = otro.pistas[i];
            }
        }
        return *this;
    }

    ~Superficie() {
        delete[] pistas;
    }

    int getNumPistas() const {
        return numPistas;
    }

    Pista* getPistas() const {
        return pistas;
    }
};

// Definición de la clase Plato
class Plato {
private:
    int numSuperficies;
    Superficie* superficies;

public:
    // Constructor predeterminado
    Plato() : numSuperficies(0), superficies(nullptr) {}

    // Constructor con parámetros
    Plato(int superficies, int pistas, int sectores) : numSuperficies(superficies) {
        this->superficies = new Superficie[numSuperficies];
        for (int i = 0; i < numSuperficies; ++i) {
            this->superficies[i] = Superficie(pistas, sectores);
        }
    }

    // Constructor de copia
    Plato(const Plato& otro) : numSuperficies(otro.numSuperficies) {
        superficies = new Superficie[numSuperficies];
        for (int i = 0; i < numSuperficies; ++i) {
            superficies[i] = otro.superficies[i];
        }
    }

    // Operador de asignación
    Plato& operator=(const Plato& otro) {
        if (this != &otro) {
            delete[] superficies;
            numSuperficies = otro.numSuperficies; // Corregir la asignación aquí
            superficies = new Superficie[numSuperficies];
            for (int i = 0; i < numSuperficies; ++i) {
                superficies[i] = otro.superficies[i];
            }
        }
        return *this;
    }



    ~Plato() {
        delete[] superficies;
    }

    int getNumSuperficies() const {
        return numSuperficies;
    }

    Superficie* getSuperficies() const {
        return superficies;
    }
};


// Definición de la clase DiscoDuro
class DiscoDuro {
private:
    int numPlatos;
    Plato* platos;
    int contadorPlatos;

public:
    // Constructor
    DiscoDuro(int platos) : numPlatos(platos), contadorPlatos(0) {
        this->platos = new Plato[numPlatos];
    }

    ~DiscoDuro() {
        delete[] platos;
    }

    // Método para agregar un nuevo plato
    void agregarPlato(const Plato& plato) {
        if (contadorPlatos < numPlatos) {
            platos[contadorPlatos] = plato;
            contadorPlatos++;
        } else {
            cout << "No se pueden agregar más platos. Capacidad máxima alcanzada." << endl;
        }
    }

    // Método para simular la creación de carpetas
    void crearCarpetas() {
        for (int p = 0; p < contadorPlatos; ++p) {
            string carpetaPlato = "Plato_" + to_string(p);
            mkdir(carpetaPlato.c_str(), 0777); // Permisos: lectura, escritura y ejecución para todos

            int numSuperficies = platos[p].getNumSuperficies();
            Superficie* superficies = platos[p].getSuperficies();
            for (int s = 0; s < numSuperficies; ++s) {
                string carpetaSuperficie = carpetaPlato + "/Superficie_" + to_string(s);
                mkdir(carpetaSuperficie.c_str(), 0777);

                int numPistas = superficies[s].getNumPistas();
                Pista* pistas = superficies[s].getPistas();
                for (int t = 0; t < numPistas; ++t) {
                    string carpetaPista = carpetaSuperficie + "/Pista_" + to_string(t);
                    mkdir(carpetaPista.c_str(), 0777);

                    int numSectores = pistas[t].getNumSectores();
                    for (int u = 0; u < numSectores; ++u) {
                        string carpetaSector = carpetaPista + "/Sector_" + to_string(u);
                        mkdir(carpetaSector.c_str(), 0777);

                        // Crear archivo de datos para el sector
                        string archivo = carpetaSector + "/sector.txt";
                        ofstream fout(archivo);
                        fout << "Datos del sector " << to_string(u) << endl;
                        fout.close();
                    }
                }
            }
        }
        cout << "Carpetas creadas exitosamente." << endl;
    }

    // Método para simular la eliminación de carpetas
    void eliminarCarpetas() {
        string comandoEliminar = "rm -rf Plato_*"; // Comando para eliminar todas las carpetas y su contenido
        system(comandoEliminar.c_str());
        cout << "Carpetas eliminadas exitosamente." << endl;
    }
};

int main() {
    int numPlatos, numSuperficies, numPistas, numSectores;

    cout << "Ingrese el número de platos: ";
    cin >> numPlatos;
    cout << "Ingrese el número de superficies por plato: ";
    cin >> numSuperficies;
    cout << "Ingrese el número de pistas por superficie: ";
    cin >> numPistas;
    cout << "Ingrese el número de sectores por pista: ";
    cin >> numSectores;

    DiscoDuro disco(numPlatos);

    for (int i = 0; i < numPlatos; ++i) {
        disco.agregarPlato(Plato(numSuperficies, numPistas, numSectores));
    }

    int opcion;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Crear carpetas" << endl;
        cout << "2. Eliminar carpetas" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                disco.crearCarpetas();
                break;
            case 2:
                disco.eliminarCarpetas();
                break;
            case 3:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
        }
    } while (opcion != 3);

    return 0;
}
