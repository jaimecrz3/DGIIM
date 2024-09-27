// Fichero: subimagen.cpp
// Genera, por copia, una imagen PGM a partir de otra imagen
//PGM. La imagen generada será, necesariamente, de un tamaño menor o
// igual que la original.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){
    char *fich_orig, *fich_rdo;     //Nombre de los ficehros de origen y destino(output/nombre.pgm)
    int fila, col;
    int filas_sub, cols_sub;

    Image image;

    // Comprobar validez de la llamada
    if (argc != 7){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: subimagen <FichImagenOriginal> <FichImagenDestino> <FilaOriginal> <ColOriginal> <AnchoNuevo> <LargoNuevo>\n";
        exit (1);
    }

    // Obtener argumentos
    fich_orig = argv[1];
    fich_rdo = argv[2];

    fila = stoi(argv[3]);
    col = stoi(argv[4]);

    filas_sub = stoi(argv[5]);
    cols_sub = stoi(argv[6]);

    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << fich_orig << endl;
    cout << "Fichero resultado: " << fich_rdo << endl;

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

    // Recortar la Imagen
    Image result = image.Crop(fila, col, filas_sub, cols_sub);

    // Guardar la imagen resultado en el fichero
    if (result.Save(fich_rdo))
        cout  << "La imagen se guardo en " << fich_rdo << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

}