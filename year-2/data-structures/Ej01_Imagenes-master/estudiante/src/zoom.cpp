// Fichero: zoom.cpp
// Realizar un zoom 2X de una porción de la imagen
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){

    char *fich_orig, *fich_rdo; // nombres de los ficheros
    int fila, col, lado; //Coordenada superior izq y lado del cuadrado
    Image image;

    // Comprobar validez de la llamada
    if (argc != 6){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: zoom <FichImagenOriginal> <FichImagenDestino> <FilaSupIzq> <ColSupIzq> <LadoCuadrado> \n";
        exit (1);
    }

    // Obtener argumentos
    fich_orig = argv[1];
    fich_rdo = argv[2];

    fila = stoi(argv[3]);
    col = stoi(argv[4]);

    lado = stoi(argv[5]);

    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << fich_orig << endl;
    cout << "Fichero resultado: " << fich_rdo << endl;
    cout << "Esquina superior izq: " << fila << endl;
    cout << "Esquina superior der: " << col << endl;
    cout << "Lado del cuadrado: " << lado << endl;

    // Leer la imagen del fichero de entrada
    if (!image.Load(fich_orig)){
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << fich_orig << ":" << endl;
    cout << "   Imagen   = " << image.get_rows()  << " filas x " << image.get_cols() << " columnas " << endl;

    // Calcular el zoom

    //Nos quedamos con la subimagen que nos interesa
    Image subimagen = image.Crop(fila, col, lado, lado);

    //Le aplicamos el zoom a la subimagen
    Image zoom = subimagen.Zoom2X();

    // Guardar la imagen resultado en el fichero
    if (zoom.Save(fich_rdo))
        cout  << "La imagen se guardo en " << fich_rdo << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    return 0;
}