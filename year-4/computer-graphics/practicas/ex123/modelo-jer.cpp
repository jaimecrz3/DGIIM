// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestion de Submarino (implementacion).  Clase 'Submarino' y relacionadas.
// ** Jaime Corzo Galdó
// ** email: jcggranada04@gmail.com, DNI: 77559045D
// **
// ** Implementacion de: 
// **     + Clase 'Submarino' (derivada de 'NodoGrafoEscena')
// **     + Clase 'Esferas' (para el cuerpo del submarino y el espejo del periscopio)
// **     + Clase 'Cilindro' (para el palo del periscopio)
// **     + Clase 'EstrellaZ' (para la helice)
// **
// *********************************************************************
 
#include "modelo-jer.h"

using namespace std;
using namespace glm;

// *********************************************************************
Esferas::Esferas
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
){

   float radio = 1;
   std::vector<glm::vec3> perfil;
   perfil.push_back({0.0, -radio, 0.0});
   for(int i=0; i<num_verts_per; i++){
      float tita = (M_PI*(i+1))/(num_verts_per);

      //matriz de giro con respecto al eje z
      vector<vector<float>> turn_matrix_z= 
      {  {cos(tita), -sin(tita), 0.0}, 
         {sin(tita), cos(tita), 0.0},
         {0.0, 0.0, 1.0},
      };

      //Nuevo vertice es turn_matrix_y*perfil[j]
      glm::vec3 aux = {turn_matrix_z[0][0] * 0.0 + turn_matrix_z[0][1] * -radio + turn_matrix_z[0][2] * 0.0,
                     turn_matrix_z[1][0] * 0.0 + turn_matrix_z[1][1] * -radio + turn_matrix_z[1][2] * 0.0,
                     turn_matrix_z[2][0] * 0.0 + turn_matrix_z[2][1] * -radio + turn_matrix_z[2][2] * 0.0, 
                  };
      perfil.push_back(aux);
   }

   inicializar(perfil, nperfiles);

}


// *********************************************************************
CilindroPeriscopio::CilindroPeriscopio
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
){
   
   //ponerNombre("palo del periscopio");
   std::vector<glm::vec3> perfil = vector<glm::vec3>();
   for(int i=0; i<num_verts_per; i++){
      float distancia_entre_puntos = i*(1.0/(num_verts_per-1)); //vec3 son de tipo float
      glm::vec3 aux = {1.0,distancia_entre_puntos,0.0};
      perfil.push_back(aux);
   }

   inicializar(perfil, nperfiles);


}

// *********************************************************************
EstrellaZHelice::EstrellaZHelice(){
    //ponerNombre("helice");
    unsigned n=6;
    float x_centro = 0.5;
    float y_centro = 0.5;
    float z_centro = 0.0;
    float radio = 0.5;
    float angulo = (2*M_PI*radio)/n;

    for(unsigned i=0; i<2*n; i++){
        if(i%2==0){
            vertices.push_back({x_centro + radio*cos(angulo*i), y_centro + radio*sin(angulo*i), z_centro});
        }else{
            vertices.push_back({x_centro + (radio/2.0)*cos(angulo*i), y_centro + (radio/2.0)*sin(angulo*i), z_centro});
        }
    }

    vertices.push_back({x_centro, y_centro, z_centro});

    for(unsigned i=0; i<2*n; i++){
        triangulos.push_back({i,(i+1)%(2*n), 2*n}); //El modulo 2*n solo es par el ultimo caso cuando i+1=2*n
    }


}

// ****************************************************************************
// Clase 'Cubo

CajaPeriscopio::CajaPeriscopio()
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}

// ****************************************************************************
// Clase TrianguloAla

TrianguloAla::TrianguloAla()
{

   vertices =
      {  { 0.5, 0.0, 0.0 }, // 0
         { -0.5, 0.0, 0.0 }, // 1
         { 0.0, sqrt(2), 0.0 } //2
      } ;



   triangulos =
      {  {0,1,2}
      } ;

}

// ****************************************************************************
// Clase MallaCuadrado

CuadradoAla::CuadradoAla()
{

   vertices =
      {  { 1.0, -1.0, 0.0 }, // 0
         { -1.0, -1.0, 0.0 }, // 1
         { 1.0, 1.0, 0.0 }, // 2
         { -1.0, 1.0, 0.0 }, // 3
      } ;



   triangulos =
      {  {0,1,2},
         {1,2,3}
      } ;

}

// *********************************************************************
// Tenemos una clase derivada de Objeto3D(NodoGradoEscena es una clase derivada de Objeto3D)
//que representa un objeto parametrizado. Esto quiere decir que cada instancia de Submarino tiene
//un numero n>0 de parametros o grados de libertad(en este caso n=3, ya que asumimos que todas las instancias
//tienen todos los parametros).
//
//A cada parámetro se le identifica por un índice de parámetro, que es un entero entre 0 y n−1. 
//
//Además, cada instancia de Submarino tiene asociado un valor de tiempo (ti) para cada parámetro i.
//El valor de tiempo es un valor real en unidades de segundos, no necesariamente el mismo para cada parámetro.
//Actualizar el estado del parámetro i al valor de tiempo ti: modificar una instancia (la geometría de 
//la instancia), en función del índice i de un parámetro y el valor de tiempo actual ti de dicho parámetro i
//
//
//En el constructor se crean los subárboles del nodo raiz y se van añadiendo a la lista de entradas.
Submarino::Submarino(){

    //indice que identifica a cada objeto
    //creo que se usa para el metodo de objeto3D, buscarObjeto
    unsigned indice_param = 1;

    ponerNombre("submarino");
    NodoGrafoEscena *submarino = new NodoGrafoEscena();

    //Cuerpo del submarino
    //Esfera radio 1
    NodoGrafoEscena *cuerpo = new NodoGrafoEscena();
    cuerpo->ponerNombre("cuerpo del sumarino");
    cuerpo->ponerColor({1.0,1.0,0.0}); //Amarillo
    cuerpo->ponerIdentificador(indice_param);
    indice_param++;
    //Como un submarino no es una esfera, sino que esta mas alargada, la escalamos en el eje z
    cuerpo->agregar(scale(vec3{2.0, 2.0, 5})); 
    cuerpo->agregar(new Esferas(50,100));

    //Periscopio
    NodoGrafoEscena *periscopio = new NodoGrafoEscena();
    //El periscopio esta formado por:
    //El palo del periscopio
    NodoGrafoEscena *palo = new NodoGrafoEscena();
    palo->ponerNombre("palo del periscopio");
    palo->ponerColor({1.0,1.0,0.0}); //amarillo
    palo->ponerIdentificador(indice_param);
    indice_param++;
    //Como la esfera esta centrada en el origen y tiene radio=2, el palo del periscopio lo trasladamos 2 unidades en el eje y
    palo->agregar(translate(vec3{0.0, 2.0, 0.0})); 
    palo->agregar(scale(vec3{0.45,1.0,0.45}));
    palo->agregar(new CilindroPeriscopio(10,100));

    //el espejo del periscopio
    NodoGrafoEscena *espejo = new NodoGrafoEscena();
    espejo->ponerNombre("espejo del periscopio");
    espejo->ponerColor({1.0,0.0,0.0}); //rojo
    espejo->ponerIdentificador(indice_param);
    indice_param++;
    //Como el cilindro tiene altura=1, trasladamos la esfera que sirve como espejo 1 unidades en el eje y, por encima de la circunferencia
    //2.75 = 2(cuerpo) + 1.0(palo)
    espejo->agregar(translate(vec3{0.0, 3.0, 0.0})); 
    espejo->agregar(scale(vec3{0.45,0.45, 0.45}));
    espejo->agregar(new Esferas(50,100));

    //la caja que rodea al espejo
    NodoGrafoEscena *caja_espejo= new NodoGrafoEscena();
    caja_espejo->ponerNombre("caja del periscopio");
    caja_espejo->ponerColor({0.0,0.0,0.0}); //negro
    caja_espejo->ponerIdentificador(indice_param);
    indice_param++;
    //Agregamos primero la rotacion para que sea lo ultimo en ejecutarse
    unsigned ind_rot_espejo = caja_espejo->agregar(rotate(0.0f,vec3{0.0, 1.0, 0.0}));

    caja_espejo->agregar(translate(vec3{0.0, 3.225, -0.25}));
    caja_espejo->agregar(scale(vec3{0.45,0.225,0.22}));
    caja_espejo->agregar(new CajaPeriscopio());

    //Agregamos los elementos del periscopio
    periscopio->agregar(palo);
    periscopio->agregar(espejo);
    periscopio->agregar(caja_espejo);

    //Helice
    NodoGrafoEscena *helice = new NodoGrafoEscena();
    helice->ponerNombre("helice");
    helice->ponerColor({1.0,0.8,0.0}); //amarillo
    helice->ponerIdentificador(indice_param);
    indice_param++;

    //Cuando se ejecuten los moviemientos que hemos agregado, se ejcutan desde el ultimo al primero
    //Si escribimos primero l traslate, luego se va a ejecutar primero la rotacion, y luego la traslacion y no es lo que queremos
    unsigned ind_rot_helice = helice->agregar(rotate(0.0f,vec3{0.0,0.0,1.0})); //la helice inicia rotando respecto al eje
    helice->agregar(translate(vec3{-0.5, -0.5, -5.0}));
    helice->agregar(new EstrellaZHelice());

    //Alas
    NodoGrafoEscena *alas = new NodoGrafoEscena();
    alas->ponerColor({1.0,0.8,0.0});
    //Ala 1:
    NodoGrafoEscena *ala1 = new NodoGrafoEscena();
    NodoGrafoEscena *cuadradoAla1 = new NodoGrafoEscena();
    cuadradoAla1->ponerNombre("cuadrado del primer ala");
    cuadradoAla1->ponerIdentificador(indice_param);
    indice_param++;

    cuadradoAla1->agregar(translate(vec3{2.5, 0.0, 0.0}));
    cuadradoAla1->agregar(scale(vec3{0.5,1.0,1.0}));
    cuadradoAla1->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));
    //cuadradoAla1->agregar(rotate(float(M_PI/2),vec3{0.0,0.0,1.0}));
    cuadradoAla1->agregar(new CuadradoAla());

    NodoGrafoEscena *trianguloAla1 = new NodoGrafoEscena();
    trianguloAla1->ponerNombre("triangulo del primer ala");
    trianguloAla1->ponerIdentificador(indice_param);
    indice_param++;

    unsigned ind_tras_ala1 = trianguloAla1->agregar(rotate(0.0f, vec3{0.0, 0.0, 1.0}));

    trianguloAla1->agregar(translate(vec3{3.0, 0.0, 0.0}));
    trianguloAla1->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));
    trianguloAla1->agregar(rotate(float(-M_PI/2),vec3{0.0,0.0,1.0}));
    trianguloAla1->agregar(scale(vec3{2.0,2.0,2.0}));
    trianguloAla1->agregar(new TrianguloAla());

    //agregamos el triangulo y el cuadrado al ala1
    ala1->agregar(cuadradoAla1);
    ala1->agregar(trianguloAla1);

    //Ala 2:
    NodoGrafoEscena *ala2 = new NodoGrafoEscena();
    NodoGrafoEscena *cuadradoAla2 = new NodoGrafoEscena();
    cuadradoAla2->ponerNombre("cuadrado del segundo ala");
    cuadradoAla2->ponerColor({1.0, 0.8, 0.0});
    cuadradoAla2->ponerIdentificador(indice_param);
    indice_param++;

    cuadradoAla2->agregar(translate(vec3{-2.5, 0.0, 0.0}));
    cuadradoAla2->agregar(scale(vec3{0.5,1.0,1.0}));
    cuadradoAla2->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));
    //cuadradoAla2->agregar(rotate(float(M_PI/2),vec3{0.0,0.0,1.0}));
    cuadradoAla2->agregar(new CuadradoAla());

    NodoGrafoEscena *trianguloAla2 = new NodoGrafoEscena();
    trianguloAla2->ponerNombre("triangulo del segundo ala");
    trianguloAla2->ponerColor({1.0, 0.8, 0.0});
    trianguloAla2->ponerIdentificador(indice_param);
    indice_param++;

    unsigned ind_tras_ala2 = trianguloAla2->agregar(rotate(0.0f, vec3{0.0, 0.0, 1.0}));

    trianguloAla2->agregar(translate(vec3{-3.0, 0.0, 0.0}));
    trianguloAla2->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));
    trianguloAla2->agregar(rotate(float(M_PI/2),vec3{0.0,0.0,1.0}));
    trianguloAla2->agregar(scale(vec3{2.0,2.0,2.0}));
    trianguloAla2->agregar(new TrianguloAla());

    //Agregamos el cuadrado y el triangulo al ala2
    ala2->agregar(cuadradoAla2);
    ala2->agregar(trianguloAla2);

    alas->agregar(ala1);
    alas->agregar(ala2);


    //Para acabar añadimos los nodos del submarino al mismo e inicializamos los parametros o grados de libertad 
    unsigned ind_tras_cuerpo = submarino->agregar(translate(vec3{0.0, 0.0, 0.0}));
    unsigned ind_rot_cuerpo = submarino->agregar(rotate(0.0f, vec3{0.0, 1.0, 0.0}));
    //en el orden en el que se añadan se crean los indices
    submarino->agregar(cuerpo);
    submarino->agregar(periscopio);
    submarino->agregar(helice);
    submarino->agregar(alas);

    pm_rot_espejo = caja_espejo->leerPtrMatriz(ind_rot_espejo);
    pm_rot_helice = helice->leerPtrMatriz(ind_rot_helice);
    pm_tras_ala1 = trianguloAla1->leerPtrMatriz(ind_tras_ala1);
    pm_tras_ala2 = trianguloAla2->leerPtrMatriz(ind_tras_ala2);

    agregar(submarino);
    pm_tras_cuerpo = submarino->leerPtrMatriz(ind_tras_cuerpo);
    pm_rot_cuerpo = submarino->leerPtrMatriz(ind_rot_cuerpo);


}

unsigned Submarino::leerNumParametros() const{
    return 4; //numero de elementos que se mueven de forma independiente
}

void Submarino::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    switch(iParam){
        case 0:
            {
                float a = 0.0;
                float b = (2.0*M_PI)/25; //frecuencia = 1/25 ciclos por segundo
                *pm_rot_espejo = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
            }
            break;
            
        case 1:
            {
                float a = 0.0;
                float b = (2.0*M_PI); //frecuencia = 1 ciclos por segundo
                *pm_rot_helice = rotate(a+(b*t_sec), vec3{0.0,0.0,1.0});
            }
            break;
        case 2:
            {
                float a_lineal = 0.0;
                float b_lineal = (2.0*M_PI)/1000; //frecuencia = 1/1000 ciclos por segundo

                float vmin = -M_PI/4;  // -45 grados en radianes
                float vmax = M_PI/4;   // 45 grados en radianes
                float a_oscilante = (vmax + vmin)/2; // Punto medio = 0
                float b_oscilante = (vmax - vmin)/2; // Amplitud = π/4
                float n = 0.5f; // Frecuencia de oscilación


                *pm_rot_cuerpo = rotate(a_lineal+(b_lineal*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_tras_cuerpo = translate(vec3{0.0, 0.0, a_oscilante + b_oscilante*sin(2*M_PI*n*t_sec)});
            }
            break;
        case 3:
            {
                float vmin = -M_PI/6;  // -45 grados en radianes
                float vmax = M_PI/6;   // 45 grados en radianes
                float a_oscilante = (vmax + vmin)/2; // Punto medio = 0
                float b_oscilante = (vmax - vmin)/2; // Amplitud = π/4
                float n = 1.0f; // Frecuencia de oscilación
                *pm_tras_ala1 = translate(vec3{3.0,0.0,0.0}) * rotate(float(a_oscilante+b_oscilante*sin(M_PI*n*t_sec)), 
                                       vec3{0.0, 0.0, 1.0}) * translate(vec3{-3.0,0.0,0.0});
                *pm_tras_ala2 = translate(vec3{-3.0,0.0,0.0}) * rotate(float(a_oscilante+b_oscilante*sin(-M_PI*n*t_sec)), 
                                          vec3{0.0, 0.0, 1.0}) * translate(vec3{3.0,0.0,0.0});
            }
            break;
    }
}