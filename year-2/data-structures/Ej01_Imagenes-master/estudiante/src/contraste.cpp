// Fichero: contraste.cpp
// Consiste en generar una imagen de niveles de gris con
// más contraste que la original.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;

int main (int argc, char *argv[]){
    char *fich_orig, *fich_rdo;     //Nombre de los ficehros de origen y destino(output/nombre.pgm)
    byte e1, e2, s1, s2;

    Image image;

    // Comprobar validez de la llamada
    if (argc != 7){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: contraste <FichImagenOriginal> <FichImagenDestino> <UmbralInfOriginal> <UmbralSuperiorOriginal> "
                "<UmbralInfOutput> <UmbralInfOutput>\n";
        exit (1);
    }

    // Obtener argumentos
    fich_orig = argv[1];
    fich_rdo = argv[2];

    int e1_, e2_, s1_, s2_;
    e1_ = stoi(argv[3]);
    e2_ = stoi(argv[4]);

    s1_ = stoi(argv[5]);
    s2_ = stoi(argv[6]);

    e1 = e1_;
    e2 = e2_;

    s1 = s1_;
    s2 = s2_;

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

    // Calculamos la imagen con distinto contraste
    image.AdjustContrast(e1, e2, s1, s2);

    // Guardar la imagen resultado en el fichero
    if (image.Save(fich_rdo))
        cout  << "La imagen se guardo en " << fich_rdo << endl;
    else{
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

}
