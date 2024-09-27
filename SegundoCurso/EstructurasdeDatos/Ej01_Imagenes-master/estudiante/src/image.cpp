/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>

#include <image.h>
#include <imageIO.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/
void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte * [rows];

    for (int i = 0; i < rows; ++i){
        img[i] = new byte [cols];
    }

    if (buffer != 0){
        for (int i = 0; i < rows*cols; ++i){
            set_pixel(i, buffer[i]);
        }
    }
}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize (int nrows, int ncols, byte * buffer){
    if ((nrows == 0) || (ncols == 0)){
        rows = cols = 0;
        img = 0;
    }
    else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen

void Image::Copy(const Image & orig){
    Initialize(orig.rows,orig.cols);
    for (int k=0; k<rows*cols;k++)
        set_pixel(k,orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const{
    return (rows == 0) || (cols == 0);
}

void Image::Destroy(){
    if (!Empty()){
        for(int i = 0; i < rows; ++i){
            delete [] img[i];
        }

        delete [] img;
    }
}

LoadResult Image::LoadFromPGM(const char * file_path){
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte * buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image(){
    Initialize();
}

// Constructores con parámetros
Image::Image (int nrows, int ncols, byte value){
    Initialize(nrows, ncols);
    for (int k=0; k<rows*cols; k++){
        set_pixel(k,value);
    }
}

bool Image::Load (const char * file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias

Image::Image (const Image & orig){
    assert (this != &orig);
    Copy(orig);
}

// Destructor

Image::~Image(){
    Destroy();
}

// Operador de Asignación

Image & Image::operator= (const Image & orig){
    if (this != &orig){
        Destroy();
        Copy(orig);
    }
    return *this;
}

// Métodos de acceso a los campos de la clase

int Image::get_rows() const {
    return rows;
}

int Image::get_cols() const {
    return cols;
}

int Image::size() const{
    return get_rows()*get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel (int i, int j, byte value) {
    img[i][j] = value;
}
byte Image::get_pixel (int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse set_pixel(i,j,value)?
    int i = k/cols;
    int j = k%cols;

    set_pixel(i, j, value);
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse get_pixel(i,j)?
    return get_pixel(k/cols, k%cols);
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    // TODO this makes assumptions about the internal representation
    byte data [rows*cols];
    for (int i = 0; i<rows*cols; ++i){
        data[i] = get_pixel(i);
    }
    return WritePGMImage(file_path, data, rows, cols);
}

void Image::Invert() {
    const int MAX_BYTE_VALUE = 255;

    for (int i=0; i < get_rows(); i++)
        for (int j=0; j < get_cols(); j++)
            set_pixel( i, j, MAX_BYTE_VALUE - get_pixel(i, j));
}

double Image::Mean(int i, int j, int height, int width) const {

    //Recorremos la matriz o submatriz indicada y calculamos la
    //suma de todos los valores
    double suma=0;
    for(int fila=i; fila<(i+height); fila++){
        for(int col=j; col<(j+width); col++){

            suma += get_pixel(fila, col);

        }
    }

    //Calculamos la media dividiendo entre el numero total de
    //pixeles sumados
    double media = suma/(height*width*1.0);

    return(media);


}
Image Image::Subsample(int factor) const {

    Image sub(int(get_rows()/factor), int(get_cols()/factor));

    for(int i=0; i<sub.get_rows(); i++){
        for(int j=0; j<sub.get_cols(); j++){

            byte nuevo = round(Mean(i*factor,j*factor ,factor, factor));
            sub.set_pixel(i, j, nuevo);

        }
    }

    return(sub);



}

Image Image::Crop(int nrow, int ncol, int height, int width) const {
    int num_columnas = get_cols();

    //Compruebo la columna
    if (ncol < 0){
        width = width + ncol;
        ncol = 0;

    } else if (ncol < num_columnas){
        if (ncol+width > num_columnas){
            width = width - ((ncol+width) - num_columnas);
        }

    } else {
        width = 0;
    }

    int num_filas = get_rows();
    //Compruebo la fila
    if (nrow < 0){
        height = height + nrow;
        nrow = 0;

    } else if (nrow < num_filas){
        if (nrow+height > num_filas){
            height = height - ((nrow+height) - num_filas);
        }

    } else {
        height = 0;

    }

    Image result(height, width);

    for (int f = 0; f < height; f++){
        for (int c = 0; c < width; c++){
            result.set_pixel(f, c, get_pixel(nrow+f, ncol+c));
        }
    }

    return (result);
}

Image Image::Zoom2X() const {

    //Creamos una imagen con 2n-1 columnas y la completamos
    Image zoom_(get_rows(), (get_cols() * 2) - 1);

    for(int j=0; j<zoom_.get_cols(); ++j) {

        if ((j % 2) == 0){
            //Rellenamos las columnas antiguas
            for (int i = 0; i < zoom_.get_rows(); ++i) {

                zoom_.set_pixel(i, j, get_pixel(i , (j / 2)));

            }

        } else {
            //Rellenamos las columnas nuevas interpolando

            for (int i = 0; i < zoom_.get_rows(); ++i){
                zoom_.set_pixel(i, j, round((get_pixel(i, (j-1)/2) + get_pixel(i, (j+1)/2)) / 2.0));
            }

        }
    }

    //Ahora creamos la imagen definitiva y repetimos el proceso
    Image zoom((get_rows()*2) - 1, zoom_.get_cols());

    //Copiamos las filas de zoom_ en zoom
    for(int i = 0; i < zoom.get_rows(); i++) {

        if((i%2)==0){

            for (int j = 0; j < zoom.get_cols(); j++) {

                zoom.set_pixel(i, j, zoom_.get_pixel((i / 2), j ));

            }

        } else {
            //Interpolamos las filas restantes

            for (int j = 0; j < zoom.get_cols(); j++) {
                if(j%2 == 0){
                    //Fila y columna impares, tomaremos el valor medio de sus vecinos
                    zoom.set_pixel(i, j, round((zoom_.get_pixel((i - 1) / 2, j) + zoom_.get_pixel((i + 1) / 2, j)) / 2.0));
                } else {
                    //Fila y columna pares, tomaremos la media de los cuatro vecinos de sus diagonales
                    int suma =   zoom_.get_pixel((i-1)/2, j-1) + zoom_.get_pixel((i-1)/2, j+1) +
                                 zoom_.get_pixel((i+1)/2, j-1) + zoom_.get_pixel((i+1)/2, j+1);

                    zoom.set_pixel(i, j, round(suma/4.0));

                }

            }
        }
    }

    return (zoom);
}

void Image::AdjustContrast (byte in1, byte in2, byte out1, byte out2){
    bool pre1 = ((0 <= in1) && (in1 <= 255) &&
                (0 <= in2) && (in2 <= 255) &&
                (0 <= out1) && (out1 <= 255) &&
                (0 <= out2) && (out2 <= 255));
    bool pre2 = (in1 < in2);
    bool pre3 = (out1 < out2);


    if (pre1 && pre2 && pre3){
        //Calculamos la parte constante e independiente del valor
        // de cada píxel a transformar
        const double TRAMO1 = 1.0*out1/in1;
        const double TRAMO2 = 1.0*(out2 - out1)/(in2 - in1);
        const double TRAMO3 = 1.0*(255 - out2)/(255 - in2);

        //Recorremos toda la imagen y en función de en que tramo de
        //la escala de grises se encuentre el byte, calculamos
        // el nuevo byte usando una transformación lineal
        for(int k=0; k<size(); k++){

            if(get_pixel(k) < in1 ){

                set_pixel(k, round((TRAMO1 * get_pixel(k) )));

            }

            else if(get_pixel(k) <= in2){

                set_pixel(k, round(out1 + (TRAMO2 * (get_pixel(k)-in1) )));

            }

            else{

                set_pixel(k, round(out2 + (TRAMO3 * (get_pixel(k)-in2) )));

            }

        }
    }




}

void Image::ShuffleRows() {

    const int p = 9973;

    byte ** temp = new byte * [rows];
    for (int i = 0; i < rows; ++i){
        temp[i] = img[i];
    }

    int newr;
    for (int r=0; r<rows; r++){

        newr = r*p % rows;
        img[r] = temp[newr];

    }
    delete [] temp;
    temp = 0;

}

