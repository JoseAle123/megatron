#include <iostream>
#include <string>
#include <sstream>

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

int main() {
    std::string cadenaEjemplo = "nombre#string#peso#int#";
    int cantidadAtributos = contarAtributos(cadenaEjemplo);
    std::cout << "La cantidad de atributos es: " << cantidadAtributos << std::endl;
    
    const int tamanoArreglo = cantidadAtributos; // Definir el tamaño del arreglo
    std::string tiposDato[tamanoArreglo];
    obtenerTiposDeDato(cadenaEjemplo, tiposDato, tamanoArreglo);
    
    std::cout << "Tipos de dato encontrados en las posiciones pares: ";
    for (int i = 0; i < cantidadAtributos; ++i) {
        std::cout << tiposDato[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
