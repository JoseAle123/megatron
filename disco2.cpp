#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class Sector {
public:
    int sectorNumber;
    int sizeInBytes;
};


class Pista {
public:
    int pistaNumber;
    int numSectores; // Nuevo miembro para almacenar el número de sectores
    Sector *sectores;

    Pista() : pistaNumber(0), numSectores(0), sectores(nullptr) {}

    Pista(int numSectores, int tamanoSector) : pistaNumber(0), numSectores(numSectores) {
        sectores = new Sector[numSectores];
        for (int i = 0; i < numSectores; ++i) {
            sectores[i].sizeInBytes = tamanoSector;
        }
    }

    // Constructor de copia para Pista
    Pista(const Pista& other) : pistaNumber(other.pistaNumber), numSectores(other.numSectores) {
        sectores = new Sector[numSectores];
        for (int i = 0; i < numSectores; ++i) {
            sectores[i] = other.sectores[i]; // Llamada al constructor de copia de Sector
        }
    }

    ~Pista() {
        delete[] sectores;
    }
};

class Superficie {
public:
    int superficieNumber;
    int numPistas;
    int numSectores; // Nuevo miembro para almacenar el número de sectores
    Pista *pistas;

    Superficie() : superficieNumber(0), numPistas(0), numSectores(0), pistas(nullptr) {}

    Superficie(int numPistas, int numSectores, int tamanoSector) : superficieNumber(0), numPistas(numPistas), numSectores(numSectores) {
        pistas = new Pista[numPistas];
        for (int i = 0; i < numPistas; ++i) {
            pistas[i] = Pista(numSectores, tamanoSector);
        }
    }

    // Constructor de copia para Superficie
    Superficie(const Superficie& other) : superficieNumber(other.superficieNumber), numPistas(other.numPistas), numSectores(other.numSectores) {
        pistas = new Pista[numPistas];
        for (int i = 0; i < numPistas; ++i) {
            pistas[i] = other.pistas[i]; // Llamada al constructor de copia de Pista
        }
    }

    ~Superficie() {
        delete[] pistas;
    }
};

class Plato {
public:
    int numSuperficies;
    Superficie *superficies;

    Plato() : numSuperficies(0), superficies(nullptr) {}

    Plato(int numSuperficies, int numPistas, int numSectores, int tamanoSector) : numSuperficies(numSuperficies) {
        superficies = new Superficie[numSuperficies];
        for (int i = 0; i < numSuperficies; ++i) {
            superficies[i] = Superficie(numPistas, numSectores, tamanoSector);
        }
    }

    // Constructor de copia para Plato
    Plato(const Plato& other) : numSuperficies(other.numSuperficies) {
        superficies = new Superficie[numSuperficies];
        for (int i = 0; i < numSuperficies; ++i) {
            superficies[i] = other.superficies[i]; // Llamada al constructor de copia de Superficie
        }
    }

    ~Plato() {
        delete[] superficies;
    }
};



void crearCarpetas(Plato* platos, int contadorPlatos) {
    for (int p = 0; p < contadorPlatos; ++p) {
        string carpetaPlato = "Plato_" + to_string(p);
        mkdir(carpetaPlato.c_str(), 0777); // Permisos: lectura, escritura y ejecución para todos

        int numSuperficies = platos[p].numSuperficies;
        Superficie* superficies = platos[p].superficies;
        for (int s = 0; s < numSuperficies; ++s) {
            string carpetaSuperficie = carpetaPlato + "/Superficie_" + to_string(s);
            mkdir(carpetaSuperficie.c_str(), 0777);

            int numPistas = superficies[s].numPistas;
            Pista* pistas = superficies[s].pistas;
            for (int t = 0; t < numPistas; ++t) {
                string carpetaPista = carpetaSuperficie + "/Pista_" + to_string(t);
                mkdir(carpetaPista.c_str(), 0777);

                int numSectores = pistas[t].numSectores;
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

int main() {
    int numPlatos, numSuperficies, numPistas, numSectores, tamanoSector;

    cout << "Ingrese el número de platos: ";
    cin >> numPlatos;
    cout << "Ingrese el número de superficies por plato: ";
    cin >> numSuperficies;
    cout << "Ingrese el número de pistas por superficie: ";
    cin >> numPistas;
    cout << "Ingrese el número de sectores por pista: ";
    cin >> numSectores;
    cout << "Ingrese el tamaño de cada sector (en bytes): ";
    cin >> tamanoSector;

    Plato* platos = new Plato[numPlatos];
    for (int i = 0; i < numPlatos; ++i) {
        platos[i] = Plato(numSuperficies, numPistas, numSectores, tamanoSector);
    }

    crearCarpetas(platos, numPlatos);

    delete[] platos;

    return 0;
}
