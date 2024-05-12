#include <iostream>
#include <limits>
#include <sys/stat.h> // Para mkdir en sistemas Unix-like
#include <fstream>
#include <sstream>
#include <iomanip>

#include <algorithm>
#include <cctype>
using namespace std;

class Sector {
private:
    int tamanio;

public:
    // Constructor predeterminado
    Sector() : tamanio(0) {}

    // Constructor con un argumento
    Sector(int tam) : tamanio(tam) {}

    int getTamanio() const {
        return tamanio;
    }
};

class Superficie {
private:
    int numeroSectores;
    Sector* sectores;

public:
    // Constructor predeterminado
    Superficie() : numeroSectores(0), sectores(nullptr) {}

    // Constructor con argumentos
    Superficie(int numSect, int tamSect) : numeroSectores(numSect) {
        sectores = new Sector[numeroSectores];
        for (int i = 0; i < numeroSectores; ++i) {
            sectores[i] = Sector(tamSect);
        }
    }

    // Constructor de copia
    Superficie(const Superficie& other) : numeroSectores(other.numeroSectores) {
        sectores = new Sector[numeroSectores];
        for (int i = 0; i < numeroSectores; ++i) {
            sectores[i] = other.sectores[i];
        }
    }

    // Operador de asignación
    Superficie& operator=(const Superficie& other) {
        if (this != &other) {
            delete[] sectores;
            numeroSectores = other.numeroSectores;
            sectores = new Sector[numeroSectores];
            for (int i = 0; i < numeroSectores; ++i) {
                sectores[i] = other.sectores[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Superficie() {
        delete[] sectores;
    }

    int getNumeroSectores() const {
        return numeroSectores;
    }

    int getTamanioSector() const {
        return (numeroSectores > 0) ? sectores[0].getTamanio() : 0;
    }
};

class Pistas {
private:
    int numeroSuperficies;
    Superficie* superficies;

public:
    Pistas() : numeroSuperficies(0), superficies(nullptr) {}

    Pistas(int numeroSuper, int numeroSector, int sizeSector) : numeroSuperficies(numeroSuper) {
        superficies = new Superficie[numeroSuper];
        for (int i = 0; i < numeroSuper; ++i) {
            superficies[i] = Superficie(numeroSector, sizeSector);
        }
    }

    // Constructor de copia
    Pistas(const Pistas& other) : numeroSuperficies(other.numeroSuperficies) {
        superficies = new Superficie[numeroSuperficies];
        for (int i = 0; i < numeroSuperficies; ++i) {
            superficies[i] = other.superficies[i];
        }
    }

    // Operador de asignación
    Pistas& operator=(const Pistas& other) {
        if (this != &other) {
            delete[] superficies;
            numeroSuperficies = other.numeroSuperficies;
            superficies = new Superficie[numeroSuperficies];
            for (int i = 0; i < numeroSuperficies; ++i) {
                superficies[i] = other.superficies[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Pistas() {
        delete[] superficies;
    }

    int getNumeroSuperficies() const {
        return numeroSuperficies;
    }

    Superficie& getSuperficie(int index) const {
        return superficies[index];
    }
};

class Platos{
private:
    int numeroPistas;
    Pistas *pistas;
public:
    Platos() : numeroPistas(0), pistas(nullptr) {}

    Platos(int numeroPistas, int numeroSuperficies, int numeroSectores, int tamanioSector) : 
    numeroPistas(numeroPistas){
        pistas = new Pistas[numeroPistas];
        for (int i = 0; i < numeroPistas; ++i) {
            pistas[i] = Pistas(numeroSuperficies, numeroSectores, tamanioSector);
        }
    }

    // Constructor de copia
    Platos(const Platos& other) : numeroPistas(other.numeroPistas) {
        pistas = new Pistas[numeroPistas];
        for (int i = 0; i < numeroPistas; ++i) {
            pistas[i] = other.pistas[i];
        }
    }

    // Operador de asignación
    Platos& operator=(const Platos& other) {
        if (this != &other) {
            delete[] pistas;
            numeroPistas = other.numeroPistas;
            pistas = new Pistas[numeroPistas];
            for (int i = 0; i < numeroPistas; ++i) {
                pistas[i] = other.pistas[i];
            }
        }
        return *this;
    }

    Pistas& getPista(int index) const {
        return pistas[index];
    }

    int getNumeroPistas() const {
        return numeroPistas;
    }


    // Destructor
    ~Platos() {
        delete[] pistas;
    }
};

class DiscoDuro{
private:
    int tamanio;
    int numeroPlatos;
    Platos *platos;
public:

    DiscoDuro(int numeroPlatos, int numeroPistas, int numeroSuperficies, int numeroSectores, int tamanioSector ) :
    numeroPlatos(numeroPlatos), platos(new Platos[numeroPlatos]) {
        for (int i = 0; i < numeroPlatos; ++i) {
            platos[i] = Platos(numeroPistas, numeroSuperficies, numeroSectores, tamanioSector);
        }
    }

    // Constructor de copia
    DiscoDuro(const DiscoDuro& other) : tamanio(other.tamanio), numeroPlatos(other.numeroPlatos) {
        platos = new Platos[numeroPlatos];
        for (int i = 0; i < numeroPlatos; ++i) {
            platos[i] = other.platos[i];
        }
    }

    // Operador de asignación
    DiscoDuro& operator=(const DiscoDuro& other) {
        if (this != &other) {
            delete[] platos;
            tamanio = other.tamanio;
            numeroPlatos = other.numeroPlatos;
            platos = new Platos[numeroPlatos];
            for (int i = 0; i < numeroPlatos; ++i) {
                platos[i] = other.platos[i];
            }
        }
        return *this;
    }

    Platos& getPlato(int index) const {
        return platos[index];
    }

    // Función para mostrar la información del disco duro
    void mostrarInformacion() const {
        for (int i = 0; i < numeroPlatos; ++i) {
            std::cout << "Plato " << i + 1 << ":" << std::endl;
            std::cout << "Número de pistas en el plato "<< i + 1 << " : " << platos[i].getNumeroPistas() << std::endl;

            for (int j = 0; j < platos[i].getNumeroPistas(); ++j) {
                std::cout << "Pista " << j + 1 << ":" << std::endl;
                std::cout << "Número de superficies en la pista: " << platos[i].getPista(j).getNumeroSuperficies() << std::endl;

                for (int k = 0; k < platos[i].getPista(j).getNumeroSuperficies(); ++k) {
                    std::cout << "Superficie " << k + 1 << ":" << std::endl;
                    std::cout << "Número de sectores en la superficie: " << platos[i].getPista(j).getSuperficie(k).getNumeroSectores() << std::endl;
                    std::cout << "Tamaño del sector en la superficie: " << platos[i].getPista(j).getSuperficie(k).getTamanioSector() << std::endl;
                }
            }
        }
    }

    void guardarInformacion() {
        std::ofstream archivo("informacion_disco.txt", std::ofstream::out | std::ofstream::trunc);
        if (archivo.is_open()) {
            archivo << "platos\tpistas\tsuperficies\tsectores\ttamanioSector\ttamanioDisco\n";
            archivo << getNumeroPlatos() << "\t" << platos[0].getNumeroPistas() << "\t" 
                    << platos[0].getPista(0).getNumeroSuperficies() << "\t" 
                    << platos[0].getPista(0).getSuperficie(0).getNumeroSectores() << "\t" 
                    << platos[0].getPista(0).getSuperficie(0).getTamanioSector() << "\t" 
                    << getTamanio() << "\n";
            archivo.close();
            std::cout << "Se han guardado los detalles del disco en 'informacion_disco.txt'\n";
        } else {
            std::cerr << "No se pudo abrir el archivo para escribir.\n";
        }
    }


    void cargarInformacionDesdeArchivo(const std::string& nombreArchivo, DiscoDuro& discoDuro) {
        std::ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            // Ignorar la primera línea
            archivo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int numPlatos, numPistas, numSuperficies, numSectores, tamSector, tamDisco;

            // Leer directamente los valores, sin necesidad de leer el delimitador
            archivo >> numPlatos >> numPistas >> numSuperficies 
                    >> numSectores >> tamSector >> tamDisco;

            // Actualizar el objeto discoDuro con los valores cargados
            discoDuro = DiscoDuro(numPlatos, numPistas, numSuperficies, numSectores, tamSector);
            discoDuro.setTamanio(tamDisco);

            std::cout << "Se han cargado los detalles del disco desde '" << nombreArchivo << "'.\n";
        } else {
            std::cerr << "No se pudo abrir el archivo para leer.\n";
        }
    }

   void crearCarpetas() const {
        for (int i = 0; i < numeroPlatos; ++i) {
            std::stringstream platoDir;
            platoDir << "Plato" << std::setw(2) << std::setfill('0') << i + 1;
            std::string platoDirStr = platoDir.str();

            if (mkdir(platoDirStr.c_str(), 0777) == -1) {
                std::cerr << "Error al crear el directorio '" << platoDirStr << "'\n";
                return;
            }

            for (int j = 0; j < platos[i].getNumeroPistas(); ++j) {
                std::stringstream pistaDir;
                pistaDir << platoDirStr << "/Pista" << std::setw(2) << std::setfill('0') << j + 1;
                std::string pistaDirStr = pistaDir.str();

                if (mkdir(pistaDirStr.c_str(), 0777) == -1) {
                    std::cerr << "Error al crear el directorio '" << pistaDirStr << "'\n";
                    return;
                }

                for (int k = 0; k < platos[i].getPista(j).getNumeroSuperficies(); ++k) {
                    std::stringstream superficieDir;
                    superficieDir << pistaDirStr << "/Superficie" << std::setw(2) << std::setfill('0') << k + 1;
                    std::string superficieDirStr = superficieDir.str();

                    if (mkdir(superficieDirStr.c_str(), 0777) == -1) {
                        std::cerr << "Error al crear el directorio '" << superficieDirStr << "'\n";
                        return;
                    }

                    for (int l = 0; l < platos[i].getPista(j).getSuperficie(k).getNumeroSectores(); ++l) {
                        std::stringstream sectorDir;
                        sectorDir << superficieDirStr << "/Sector" << std::setw(2) << std::setfill('0') << l + 1;
                        std::string sectorDirStr = sectorDir.str();

                        if (mkdir(sectorDirStr.c_str(), 0777) == -1) {
                            std::cerr << "Error al crear el directorio '" << sectorDirStr << "'\n";
                            return;
                        }
                    }
                }
            }
        }
        std::cout << "Estructura de carpetas creada correctamente en el directorio actual\n";
    }



    void setTamanio(int tamanio){
        this->tamanio = tamanio;
    }

    int getTamanio() const{
        return tamanio;
    }


    int getNumeroPlatos() const {
        return numeroPlatos;
    }


    // Destructor
    ~DiscoDuro() {
        delete[] platos;
    }
};

std::string getSchema(const std::string& schemaFile, std::string fileName);
int contarAtributos(const std::string& cadena);
void obtenerTiposDeDato(const std::string& cadena, std::string tiposDato[], int tamano);

class Registro {
private:
    int tamanioChar = 1;
    int tamanioInt = 4;
    int tamanioFloat = 4;
    int tamanioRegistro = 0;

public:
    Registro() {}

    void calcularTamanio(string campo, string tipoDato) {
        if (tipoDato == "string") {
            tamanioRegistro += tamanioChar * campo.length();
        } else if (tipoDato == "int") {
            tamanioRegistro += tamanioInt;
        } else if (tipoDato == "float") {
            tamanioRegistro += tamanioFloat;
        }
    }

    int getTamanioRegistro() {
        return tamanioRegistro;
    }

    void reiniciarTamanio() {
        tamanioRegistro = 0;
    }

    int calcularTamanioRegistro(){
        string nombreArchivo;
        cout << "ingrese el nombre del Archivo: "; cin >> nombreArchivo;
        std::ifstream archivo(nombreArchivo);
        string DatosEsquema = getSchema("esquemas.txt", nombreArchivo);
        cout << DatosEsquema << endl;

        int cantidadAtributos = contarAtributos(DatosEsquema);

        string tiposDato[cantidadAtributos];
        obtenerTiposDeDato(DatosEsquema, tiposDato, cantidadAtributos);


        std::string linea;

        // Ignorar la primera línea
        std::getline(archivo, linea);

        // Procesar el resto del archivo
        int contador = 0;
        while (std::getline(archivo, linea)) {
            std::istringstream ss(linea);
            std::string palabra;

            // Leer palabra por palabra
            while (std::getline(ss, palabra, '#')) {
                if(contador >= cantidadAtributos){
                    contador = 0;
                }
                calcularTamanio(palabra, tiposDato[contador]);
                cout << contador <<" "<<palabra << endl;
                contador++;
            }
            
        }
        archivo.close();
        return tamanioRegistro;


    };
};

int contarAtributos(const std::string& cadena) {
    std::istringstream ss(cadena);
    std::string fragmento;
    int contador = 0;
        
    while (std::getline(ss, fragmento, '#')) {
         if (!fragmento.empty() && fragmento != "string" && fragmento != "int") {
              contador++;
           }
       }
        
      return contador;
 }

 void obtenerTiposDeDato(const std::string& cadena, std::string tiposDato[], int tamano) {
    std::istringstream ss(cadena);
    std::string fragmento;
    int indice = 0;
    bool guardar = false;
    
    while (std::getline(ss, fragmento, '#')) {
        if (guardar && indice < tamano) {
            tiposDato[indice] = fragmento;
            indice++;
        }
        guardar = !guardar;
    }
}


string getSchema(const string& schemaFile, string fileName) {
    // Eliminar los últimos 4 caracteres (extensión .txt)
    fileName = fileName.substr(0, fileName.size() - 4);
    ifstream file(schemaFile);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, '#');
            if (name == fileName) {
                return line.substr(name.length() + 1);
            }
        }
        file.close();
    }
    return "";
}


int main() {

    DiscoDuro discoDuro(0, 0, 0, 0, 0); // Se inicializa con valores predeterminados, los cuales se sobrescribirán en la opción 1

    int opcion;
    do {
        cout << "Ingrese 1 si quiere crear el disco" << endl;
        cout << "Ingrese 2 para cargar los datos disco de un txt" << endl;
        cout << "Ingrese 3 si quiere mostrar los datos del disco" << endl;
        cout << "Ingrese 4 para crear carpetas" << endl;
        cout << "Ingrese 5 para crear registros" << endl;
        cout << "Opción: ";
        cin >> opcion;
        switch (opcion) {
            case 1: {
                int numPlatos;
                int numPistas;
                int numSuperficies;
                int numSectores;
                int tamSector;
                int tamDisco;

                cout << "Ingrese el número de platos: "; cin >> numPlatos;
                cout << "Ingrese el número de pistas: "; cin >> numPistas;
                cout << "Ingrese el número de superficies: "; cin >> numSuperficies;
                cout << "Ingrese el número de sectores: "; cin >> numSectores;
                cout << "Ingrese el tamaño del sector: ";  cin >> tamSector;
                cout << "Ingre el tamanio del disco: "; cin >> tamDisco;

                // Crear una instancia de DiscoDuro
                discoDuro = DiscoDuro(numPlatos, numPistas, numSuperficies, numSectores, tamSector);
                discoDuro.setTamanio(tamDisco);
                discoDuro.guardarInformacion();
                break;
            }
            case 2:{
                discoDuro.cargarInformacionDesdeArchivo("informacion_disco.txt", discoDuro);
                break;
            }

            case 3:
                discoDuro.mostrarInformacion();
                cout << "Tamanio del disco: " << discoDuro.getTamanio() << endl;
                break;
            case 4:{
                discoDuro.crearCarpetas();
                break;
            }
            case 5:{
                Registro registro;
                int tamanio = registro.calcularTamanioRegistro();
                cout << tamanio << endl;
                break;
            }
            default:
                break;
        }
    } while (opcion != 6);
    

    

    // Acceder a las propiedades de los platos y sus pistas
  

    // Definir los parámetros para la creación de Platos
    /* int numPlatos = 2;
    int numPistas = 3;
    int numSuperficies = 2;
    int numSectores = 3;
    int tamSector = 5;

    // Crear una instancia de Platos
    Platos platos(numPlatos, numSuperficies, numSectores, tamSector);

    // Acceder a las propiedades de Platos
    std::cout << "Número de platos: " << numPlatos << std::endl;

    // Iterar sobre cada plato y acceder a sus pistas
    for (int i = 0; i < numPlatos; ++i) {
        std::cout << "Plato " << i + 1 << ":" << std::endl;
        std::cout << "Número de pistas en el plato: " << platos.getPista(i).getNumeroSuperficies() << std::endl;

        // Iterar sobre cada pista en el plato y acceder a sus propiedades
        for (int j = 0; j < platos.getPista(i).getNumeroSuperficies(); ++j) {
            std::cout << "Pista " << j + 1 << ":" << std::endl;
            std::cout << "Número de sectores en la pista: " << platos.getPista(i).getSuperficie(j).getNumeroSectores() << std::endl;
            std::cout << "Tamaño del primer sector en la pista: " << platos.getPista(i).getSuperficie(j).getTamanioSector() << std::endl;
        }
    }

    return 0; */
}
