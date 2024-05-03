#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Para la función count
#include <cctype>


using namespace std;

class Columna {
private:
    string nombre;
    string tipo;
public:
    // Constructor predeterminado
    Columna() : nombre(""), tipo("") {}

    Columna(const string& nombreColumna, const string& tipoDato) : nombre(nombreColumna), tipo(tipoDato) {}

    string obtenerNombre() const {
        return nombre;
    }

    string obtenerTipo() const {
        return tipo;
    }
};

class Tabla {
private:
    string nombre;
    Columna* columnas;
    int numColumnas;

public:

    Tabla() : columnas(nullptr), numColumnas(0) {}
    Tabla(const string& nombreTabla) : nombre(nombreTabla), columnas(nullptr), numColumnas(0) {}

    void agregarColumna(const string& nombreColumna, const string& tipoDato) {
        Columna* nuevasColumnas = new Columna[numColumnas + 1];
        for (int i = 0; i < numColumnas; ++i) {
            nuevasColumnas[i] = columnas[i];
        }
        nuevasColumnas[numColumnas] = Columna(nombreColumna, tipoDato);
        delete[] columnas;
        columnas = nuevasColumnas;
        numColumnas++;
    }

    void guardarEnArchivo(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo, ios::app);
        if (archivo.is_open()) {
            archivo << nombre << "#"; // Separador entre nombre de tabla y columnas
            for (int i = 0; i < numColumnas; ++i) {
                archivo << columnas[i].obtenerNombre() << "#" << columnas[i].obtenerTipo() << "#"; // Separador entre columnas
            }
            archivo << endl;
            cout << "Tabla '" << nombre << "' guardada en archivo" << endl;
            archivo.close();
        } else {
            cout << "Error: archivo no abierto" << endl;
        }
    }

    void agregarRegistro(const string& nombreArchivo) const {
        ifstream archivoEsquemas("esquemas.txt");
        if (!archivoEsquemas.is_open()) {
            cout << "Error: No se pudo abrir el archivo de esquemas." << endl;
            return;
        }

        // Leer la primera línea de esquemas.txt para obtener los nombres de las columnas y sus tipos
        string lineaEsquema;
        getline(archivoEsquemas, lineaEsquema);
        istringstream ss(lineaEsquema);
        string columnaNombre, columnaTipo;
        const int MAX_COLUMNAS = 100; // Número máximo de columnas
        string nombresColumnas[MAX_COLUMNAS];
        string tiposColumnas[MAX_COLUMNAS];
        int numColumnas = 0;

        while (getline(ss, columnaNombre, '#') && getline(ss, columnaTipo, '#')) {
            nombresColumnas[numColumnas] = columnaNombre;
            tiposColumnas[numColumnas] = columnaTipo;
            numColumnas++;
        }

        archivoEsquemas.close();

        // Solicitar los valores para cada columna al usuario
        ofstream archivo(nombreArchivo, ios::app);
        if (archivo.is_open()) {
            for (int i = 0; i < numColumnas; ++i) {
                string valor;
                cout << "Ingrese el valor para " << nombresColumnas[i] << " (" << tiposColumnas[i] << "): ";
                cin >> valor;
                archivo << (i == 0 ? "" : "#") << valor;
            }
            archivo << endl;
            cout << "Registro agregado exitosamente." << endl;
            archivo.close();
        } else {
            cout << "Error: archivo no abierto" << endl;
        }
    }

    void ConsultaEdad(const string& nombre_archivo_entrada, const string NombreColumna) {
        ifstream archivo_entrada(nombre_archivo_entrada);
        string nombre_archivo_salida;
        cout << "Escriba el nombre del archivo de salida: ";
        cin >> nombre_archivo_salida;

        ofstream archivo_salida(nombre_archivo_salida);

        if (!archivo_entrada || !archivo_salida) {
            cerr << "Error al abrir archivos." << endl;
            return;
        }

        int limiteEdad;
        string operador;
        cout << "Ingrese el operador '>' '<' '>=' '<=': "; cin >> operador;
        cout << "Ingrese el límite de edad: "; cin >> limiteEdad;

        string linea;
        // Leer la primera línea del archivo de entrada (encabezado) y escribirla en el archivo de salida
        getline(archivo_entrada, linea);
        archivo_salida << linea << endl;

        // Encontrar la columna que contiene la edad
        int columnaEdad = -1;
        istringstream iss(linea);
        string columna;
        int indiceColumna = 0;
        while (getline(iss, columna, '#')) {
            if (columna == NombreColumna) {
                columnaEdad = indiceColumna;
                break;
            }
            indiceColumna++;
        }

        if (columnaEdad == -1) {
            cerr << "No se encontró la columna "<< NombreColumna << "en el encabezado." << endl;
            return;
        }

        // Escribir las líneas que cumplan con el filtro en el archivo de salida
        while (getline(archivo_entrada, linea)) {
            istringstream iss(linea);
            string valor;
            int columna = 0;
            bool cumpleFiltro = false;

            // Leer cada valor separado por el delimitador
            while (getline(iss, valor, '#')) {
                if (columna == columnaEdad) { // La columna que contiene la edad
                    int edad;
                    istringstream(valor) >> edad;
                    // Verificar si la edad cumple con el filtro
                    if (operador == ">") {
                        cumpleFiltro = (edad > limiteEdad);
                    } else if (operador == "<") {
                        cumpleFiltro = (edad < limiteEdad);
                    } else if (operador == ">=") {
                        cumpleFiltro = (edad >= limiteEdad);
                    } else if (operador == "<=") {
                        cumpleFiltro = (edad <= limiteEdad);
                    } else {
                        cerr << "Operador no válido." << endl;
                        return;
                    }
                    break; // Salir del bucle al encontrar la edad
                }
                columna++;
            }

            // Escribir la línea en el archivo de salida si cumple con el filtro
            if (cumpleFiltro) {
                archivo_salida << linea << endl;
            }
        }

        cout << "Se han guardado los datos según el filtro especificado en el archivo '" << nombre_archivo_salida << "'" << endl;

        archivo_entrada.close();
        archivo_salida.close();
    }

    void ConsultaAtributo(const string& nombre_archivo_entrada, const string NombreColumna) {
        ifstream archivo_entrada(nombre_archivo_entrada);
        string nombre_archivo_salida;
        cout << "Escriba el nombre del archivo de salida: ";
        cin >> nombre_archivo_salida;

        ofstream archivo_salida(nombre_archivo_salida);

        if (!archivo_entrada || !archivo_salida) {
            cerr << "Error al abrir archivos." << endl;
            return;
        }

        string linea;
        // Leer la primera línea del archivo de entrada (encabezado) y escribirla en el archivo de salida
        getline(archivo_entrada, linea);
        archivo_salida << linea << endl;

        // Solicitar al usuario el estado que desea consultar
        int estado;
        cout << "Ingrese el valor del atributo que desea consultar: ";
        cin >> estado;

        // Encontrar la columna que contiene el estado "Survived"
        int columnaSurvived = -1;
        istringstream iss(linea);
        string columna;
        int indiceColumna = 0;
        while (getline(iss, columna, '#')) {
            if (columna == NombreColumna) {
                columnaSurvived = indiceColumna;
                break;
            }
            indiceColumna++;
        }

        if (columnaSurvived == -1) {
            cerr << "No se encontró la columna" << NombreColumna << "en el encabezado." << endl;
            return;
        }

        // Escribir las líneas donde el estado "Survived" es igual al especificado por el usuario en el archivo de salida
        while (getline(archivo_entrada, linea)) {
            istringstream iss(linea);
            string valor;
            int columna = 0;
            bool cumpleFiltro = false;

            // Leer cada valor separado por el delimitador
            while (getline(iss, valor, '#')) {
                if (columna == columnaSurvived) {
                    int survived;
                    istringstream(valor) >> survived;
                    // Verificar si el estado "Survived" es igual al especificado por el usuario
                    cumpleFiltro = (survived == estado);
                    break; // Salir del bucle al encontrar el estado "Survived"
                }
                columna++;
            }

            // Escribir la línea en el archivo de salida si cumple con el filtro
            if (cumpleFiltro) {
                archivo_salida << linea << endl;
            }
        }

        cout << "Se han guardado los datos según el filtro especificado en el archivo '" << nombre_archivo_salida << "'" << endl;

        archivo_entrada.close();
        archivo_salida.close();
    }
   

    void mostrarDetalles() const {
        cout << "Nombre de la tabla: " << nombre << endl;
        cout << "Columnas:" << endl;
        for (int i = 0; i < numColumnas; ++i) {
            cout << "  Nombre: " << columnas[i].obtenerNombre() << ", Tipo: " << columnas[i].obtenerTipo() << endl;
        }
    }

    ~Tabla() {
        delete[] columnas;
    }
};

void mostrarEsquemas(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            istringstream ss(linea);
            string nombreTabla, columnaNombre, columnaTipo;
            getline(ss, nombreTabla, '#');
            cout << "Nombre de la tabla: " << nombreTabla << endl;
            cout << "Columnas:" << endl;
            while (getline(ss, columnaNombre, '#') && getline(ss, columnaTipo, '#')) {
                cout << "  Nombre: " << columnaNombre << ", Tipo: " << columnaTipo << endl;
            }
            cout << endl;
        }
        archivo.close();
    } else {
        cout << "Error: no se pudo abrir el archivo" << endl;
    }
}

void convertirCSVaTXT() {
    string nombreArchivoCSV;
    cout << "Ingrese el nombre del archivo CSV que desea leer ('salir' para salir): ";
    cin >> nombreArchivoCSV;

    if (nombreArchivoCSV != "salir") {
        ifstream archivoCSV(nombreArchivoCSV);
        if (archivoCSV.is_open()) {
            string nombreArchivoSalida;
            cout << "Ingrese el nombre del archivo de salida: ";
            cin >> nombreArchivoSalida;

            ofstream archivoSalida(nombreArchivoSalida, ios::app);

            string linea;
            while (getline(archivoCSV, linea)) {
                istringstream ss(linea);
                string campo;
                bool primerCampo = true;

                while (getline(ss, campo, ',')) {
                    if (campo.empty()) {
                        campo = "null";
                    }

                    if (campo.front() == '"') {
                        string campoCompleto = campo;
                        while (campo.back() != '"' && getline(ss, campo, ',')) {
                            campoCompleto += "," + campo;
                        }
                        archivoSalida << (primerCampo ? "" : "#") << campoCompleto;
                    } else {
                        archivoSalida << (primerCampo ? "" : "#") << campo;
                    }
                    primerCampo = false;
                }

                archivoSalida << endl;
            }
            archivoSalida.close();
            cout << "Archivo de salida generado exitosamente." << endl;
        } else {
            cout << "Error: No se pudo abrir el archivo CSV." << endl;
        }
    }
}

int main() {
    int opcion;
    string nombreTabla;

    do {
        cout << "MENU:" << endl;
        cout << "1. Crear una esquema" << endl;
        cout << "2. Mostrar detalles de los esquemas" << endl;
        cout << "3. Convertir CSV a TXT" << endl;
        cout << "4. Agregar nuevo registro" << endl;
        cout << "5. Consulta de edad: " << endl;
        cout << "6. Consulta de valor de atributo: " << endl;
        cout << "7. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Ingrese el nombre de la tabla ('fin' para salir): ";
                cin >> nombreTabla;

                if (nombreTabla != "fin") {
                    Tabla nuevaTabla(nombreTabla);
                    string nombreColumna, tipoDato;
                    do {
                        cout << "Ingrese el nombre de la columna ('fin' para terminar): ";
                        cin >> nombreColumna;
                        if (nombreColumna == "fin") break;
                        cout << "Ingrese el tipo de dato de la columna: ";
                        cin >> tipoDato;
                        nuevaTabla.agregarColumna(nombreColumna, tipoDato);
                    } while (true);

                    // Guardar la tabla en un archivo
                    nuevaTabla.guardarEnArchivo("esquemas.txt");
                    break;
                }
                break;
            }
            case 2: {
                mostrarEsquemas("esquemas.txt");
                break;
            }
            case 3:{
                convertirCSVaTXT();
                break;
            }
            case 4: {
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo donde desea agregar el registro: ";
                cin >> nombreArchivo;
                Tabla nuevaTabla;
                nuevaTabla.agregarRegistro(nombreArchivo);
                break;
            }
            case 5: {
                string nombreArchivo, nombreColumna;
                cout << "Ingrese el nombre del archivo donde desea consultar la edad: ";
                cin >> nombreArchivo;
                cout << "ingrese el nombre de la columna: ";
                cin >> nombreColumna;
                Tabla nuevaTabla;
                nuevaTabla.ConsultaEdad(nombreArchivo, nombreColumna);
                break;
            }
            case 6: {
                string nombreArchivo, nombreColumna;
                cout << "Ingrese el nombre del archivo donde desea saber el atributo: ";
                cin >> nombreArchivo;
                cout << "ingrese el nombre de la columna: ";
                cin >> nombreColumna;
                Tabla nuevaTabla;
                nuevaTabla.ConsultaAtributo(nombreArchivo, nombreColumna);
                break;
            }
            case 7: {
                cout << "Saliendo del programa..." << endl;
                break;
            }
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }

        // Limpiar el búfer de entrada

    } while (opcion != 7);

    return 0;
}


               
