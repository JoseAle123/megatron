#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Para la función count
#include <cctype>
#include <vector>

using namespace std;

class Columna {
private:
    string nombre;
    string tipo;
public:
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
    vector<Columna> columnas;

public:

    Tabla() {};
    Tabla(const string& nombreTabla) : nombre(nombreTabla) {}

    void agregarColumna(const string& nombreColumna, const string& tipoDato) {
        columnas.push_back(Columna(nombreColumna, tipoDato));
    }

    void guardarEnArchivo(ofstream& archivo) const {
        if (archivo.is_open()) {
            archivo << nombre << "#"; // Separador entre nombre de tabla y columnas
            for (const auto& columna : columnas) {
                archivo << columna.obtenerNombre() << "#" << columna.obtenerTipo() << "#"; // Separador entre columnas
            }
            archivo << endl;
            cout << "Tabla '" << nombre << "' guardada en archivo" << endl;
        } else {
            cout << "Error: archivo no abierto" << endl;
        }
    }

    void mostrarDetalles() const {
        cout << "Tabla: " << nombre << endl;
        cout << "Columnas:" << endl;
        for (const auto& columna : columnas) {
            cout << "- Nombre: " << columna.obtenerNombre() << ", Tipo: " << columna.obtenerTipo() << endl;
        }
    }

    void mostrarDetallesTabla(const vector<Tabla>& tablas) const {
        string nombreTabla;
        cout << "Ingrese el nombre de la tabla que desea ver: ";
        cin >> nombreTabla;
        bool encontrada = false;
        for (const auto& tabla : tablas) {
            if (tabla.obtenerNombre() == nombreTabla) {
                tabla.mostrarDetalles();
                encontrada = true;
                break;
            }
        }
        if (!encontrada) {
            cout << "No se encontró ninguna tabla con el nombre '" << nombreTabla << "'" << endl;
        }
    }

    

    string obtenerNombre() const {
        return nombre;
    }


    static vector<Tabla> cargarTablasDesdeArchivo(const string& nombreArchivo) {
        vector<Tabla> tablas;
        ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            string linea;
            while (getline(archivo, linea)) {
                stringstream ss(linea);
                string nombreTabla;
                getline(ss, nombreTabla, '#'); // Leer el nombre de la tabla
                Tabla nuevaTabla(nombreTabla);
                string columna;
                while (getline(ss, columna, '#')) {
                    string nombreColumna = columna;
                    string tipoDato;
                    getline(ss, tipoDato, '#'); // Leer el tipo de dato
                    if (!nombreColumna.empty() && !tipoDato.empty()) {
                        nuevaTabla.agregarColumna(nombreColumna, tipoDato);
                    }
                }
                tablas.push_back(nuevaTabla);
            }
            archivo.close();
        } else {
            cerr << "Error al abrir el archivo '" << nombreArchivo << "'" << endl;
        }
        return tablas;
    }
};

static string toLowerCase(const string& str) {
        string result;
        for (char c : str) {
            result += tolower(c);
        }
        return result;
}

void convertirCSVaTXT(const vector<Tabla>& tablas) {
    string nombreEsquema;
    cout << "ingrese el nombre de la esquema para agregar datos: "; cin >> nombreEsquema;

    for(const auto& tabla : tablas){
        if(nombreEsquema == tabla.obtenerNombre()){
            string nombreArchivoCSV;
            cout << "Ingrese el nombre del archivo CSV que desea leer ('salir' para salir): ";
            cin >> nombreArchivoCSV;

            if (nombreArchivoCSV != "salir") {
                ifstream archivoCSV(nombreArchivoCSV);
                if (archivoCSV.is_open()) {
                    string nombreSalida = "salida_" + nombreArchivoCSV.substr(0, nombreArchivoCSV.find_last_of(".")) + ".txt";
                    string textoMinuscula = toLowerCase(nombreSalida);

                    // Verificar si el archivo de salida ya existe
                    ifstream verificaSalida(textoMinuscula);
                    if (verificaSalida.good()) {
                        cout << "Error: El archivo de salida ya existe." << endl;
                        return;
                    }
                    verificaSalida.close();

                    ofstream archivoSalida(textoMinuscula, ios::app);

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
            return;
        }
    }
    cout << "el esquema no existe" << endl; 
}

string obtenerAtributos(string nombreTabla) {
    string atributos;

    string nombreArchivoSalida = nombreTabla;

    // Verificar si el archivo de salida existe
    ifstream verificaSalida(nombreArchivoSalida);
    if (verificaSalida.good()) {
        verificaSalida.close();

        // Leer los atributos desde el archivo de salida
        ifstream archivoSalida(nombreArchivoSalida);
        getline(archivoSalida, atributos);
        archivoSalida.close();

        cout << "Atributos existentes: " << atributos << endl;
    } else {
        verificaSalida.close();
        cout << "Error: El archivo de salida no existe." << endl;
    }

    return atributos;
}

// Función para agregar un nuevo registro al archivo de salida
void agregarNuevoRegistro(string nombreTabla, const string& atributos) {
    string nombreArchivoSalida = nombreTabla;

    // Solicitar valores para los nuevos registros
    ofstream archivoSalidaAgregar(nombreArchivoSalida, ios::app);
    string valor;
    istringstream ss(atributos);
    string atributo;
    while (getline(ss, atributo, '#')) {
        cout << "Ingrese " << atributo << ": ";
        getline(cin, valor);

        archivoSalidaAgregar << valor << "#";
    }
    archivoSalidaAgregar << endl;

    archivoSalidaAgregar.close();

    cout << "Nuevo registro agregado exitosamente." << endl;
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void ConsultaEdad(const string& nombre_archivo_entrada) {
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
    cout << "ingrese el limite de edad: "; cin >> limiteEdad;

    if (operador != ">" && operador != ">=" && operador != "<" && operador != "<=") {
        cerr << "Operador no válido. Por favor, use '>' o '>='." << endl;
        return;
    }

    string linea;
    // Leer la primera línea del archivo de entrada (encabezado) y escribirla en el archivo de salida
    getline(archivo_entrada, linea);
    archivo_salida << linea << endl;

    // Encontrar el índice de la columna "Age" en el encabezado
    vector<string> encabezado = split(linea, '#');
    int indiceEdad = -1;
    for (size_t i = 0; i < encabezado.size(); ++i) {
        if (encabezado[i] == "Age") {
            indiceEdad = static_cast<int>(i);
            break;
        }
    }

    if (indiceEdad == -1) {
        cerr << "No se encontró la columna 'Age' en el encabezado." << endl;
        return;
    }

    // Leer el resto de las líneas y filtrar las personas según la edad y el operador seleccionado
    while (getline(archivo_entrada, linea)) {
        vector<string> campos = split(linea, '#');
        if (campos.size() <= static_cast<size_t>(indiceEdad)) {
            cerr << "Error en el formato de la línea." << endl;
            continue;
        }

        try {
            int edad = stoi(campos[indiceEdad]);
            if ((operador == ">" && edad > limiteEdad) || (operador == ">=" && edad >= limiteEdad) ||
                (operador == "<" && edad < limiteEdad) || (operador == "<=" && edad <= limiteEdad)) {
                archivo_salida << linea << endl;
            }
        } catch (const invalid_argument&) {
            cerr << "Error al convertir la edad a entero." << endl;
        }
    }

    cout << "Se han guardado los datos según el filtro especificado en el archivo '" << nombre_archivo_salida << "'" << endl;

    archivo_entrada.close();
    archivo_salida.close();
}

int main() {
    vector<Tabla> tablas = Tabla::cargarTablasDesdeArchivo("esquemas.txt");
    int opcion;

    do {
        cout << "MENU:" << endl;
        cout << "1. Crear una esquema" << endl;
        cout << "2. Mostrar detalles de los esquemas" << endl;
        cout << "3. Salida de un archivo CSV a TXT" << endl;
        cout << "4. Crear un nuevo registro" << endl;
        cout << "5. Hacer una consulta" << endl;
        cout << "6. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombreTabla;
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
                    tablas.push_back(nuevaTabla);

                    // Guardar la tabla en el archivo "esquemas.txt"
                    ofstream archivo("esquemas.txt", ios::app);
                    if (archivo.is_open()) {
                        nuevaTabla.guardarEnArchivo(archivo);
                        archivo.close();
                    } else {
                        cerr << "Error al abrir el archivo 'esquemas.txt'" << endl;
                    }
                }
                break;
            }
            case 2: {
                Tabla tabla;
                tabla.mostrarDetallesTabla(tablas);
                break;
            }
            case 3: {
                convertirCSVaTXT(tablas);
                break;
            }
            case 4:{
                string archivoSalida;
                cout << "Ingrese el nombre del archivo de salida ('salir' para salir): ";
                cin >> archivoSalida;
                if(archivoSalida != "salir") {
                    cin.ignore(); // Limpiar el buffer del teclado
                    string atributos = obtenerAtributos(archivoSalida);
                    if (!atributos.empty()) {
                        agregarNuevoRegistro(archivoSalida, atributos);
                    }
                } else {
                    break;
                }
            }
            case 5:{
                int opcion2;
                cout << "1. consuta de edad" << endl;
                cout << "2. estado de atributo" << endl;
                cout << "opcion ?: "; cin >> opcion2;
                string ArchvivoEntrada;
                switch (opcion2)
                {
                case 1:
                    cout << "ingrese el archivo de entrada: "; cin >> ArchvivoEntrada;
                    ConsultaEdad(ArchvivoEntrada);
                    break;

                case 2:
                    break;
                
                default:
                    break;
                }
            }


            case 6: {
                cout << "Gracias por usar el sistema." << endl;
                break;
            }
            default:
                cout << "Opción no válida. Inténtelo de nuevo." << endl;
        }
    } while (opcion != 6);

    return 0;
}