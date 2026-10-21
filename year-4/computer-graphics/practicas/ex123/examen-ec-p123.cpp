// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#include "examen-ec-p123.h"

// ****************************************************************************
// Clase MallaTriangulo

MallaEjercicio1::MallaEjercicio1()
{
    ponerNombre("MallaEjercicio1");
   vertices =
      {  { 0.0, 1.0 ,0.0 }, // sup1: 0
         { 0.0, 1.0, -1.0 }, // sup2: 1
         { 0.5, 0.0, 0.0 }, //infdch1: 2
         { 0.5, 0.0, -1.0 }, //infdch2: 3
         { 1.5, 0.0, 0.0 }, //infdch3: 4
         { 1.5, 0.0, -1.0 }, //infdch4: 5
         { -0.5, 0.0, 0.0 }, //infizd1: 6
         { -0.5, 0.0, -1.0 }, //infizd2: 7
         { -1.5, 0.0, 0.0 }, //infizd3: 8
         { -1.5, 0.0, -1.0 }, //infizd4: 9
      } ;



   triangulos =
      {  {0,2,3},
         {0, 1,3},
         {2,3,5},
         {2,4,5},
         {0,1,6},
         {1, 6,7},
         {6, 7, 8},
         {7,8,9}
      } ;

    col_ver.push_back({1.0,1.0,1.0}); 
    col_ver.push_back({1.0,1.0,1.0});
    for(unsigned i=0; i<8; i++){
        col_ver.push_back({0.2,0.2,0.2});
    }

}

// ---------------------------------------------------------------------

MallaEjercicio2::MallaEjercicio2(unsigned n)
{
    assert(n>=1);
    ponerNombre("MallaEjercicio1");
    for(unsigned i=0; i<=n; i++){
        vertices.push_back({float(i),0.0,0.0});
        vertices.push_back({float(i),0.0,-1.0});
        vertices.push_back({float(i),1.0,-1.5});
        vertices.push_back({float(i),0.0,-2.0});
        vertices.push_back({float(i),0.0,-3.0});
        
    }


    for(unsigned i=0; i<5*n; i+=5){
        triangulos.push_back({i, i+1, i+5});
        triangulos.push_back({i+1, i+5, i+6});
        triangulos.push_back({i+1, i+2, i+6});
        triangulos.push_back({i+2, i+6, i+7});
        triangulos.push_back({i+2, i+3, i+7});
        triangulos.push_back({i+3, i+7, i+8});
        triangulos.push_back({i+3, i+4, i+8});
        triangulos.push_back({i+4, i+8, i+9});
    }

    for(unsigned i=0; i<=n; i++){
        col_ver.push_back({0.2,0.2,0.2});
        col_ver.push_back({0.2,0.2,0.2});
        col_ver.push_back({1.0,1.0,1.0});
        col_ver.push_back({0.2,0.2,0.2});
        col_ver.push_back({0.2,0.2,0.2});
    }

}

// ---------------------------------------------------------------------

MallaEjercicio3::MallaEjercicio3(unsigned w, unsigned h){

    unsigned indice = 1;

    ponerNombre("MallaEjercicio1");
    NodoGrafoEscena *prismas = new NodoGrafoEscena();

    //prisma inferior
    NodoGrafoEscena *prisma_inf = new NodoGrafoEscena();
    prisma_inf->ponerNombre("prisma inferior");
    prisma_inf->ponerIdentificador(indice);
    indice++;

    unsigned ind_tras_prisma_inf = prisma_inf->agregar(scale(vec3{1.0,1.0,1.0}));

    prisma_inf->agregar(scale(vec3{float(w)/2.0,float(h)/2.0,float(w)/2.0}));
    prisma_inf->agregar(translate(vec3{0.0,1.0,0.0}));
    prisma_inf->agregar(new Cubo());

    //prisma superior
    NodoGrafoEscena *prisma_sup = new NodoGrafoEscena();
    prisma_inf->ponerNombre("prisma superior");
    prisma_sup->ponerIdentificador(indice);
    indice++;

    unsigned ind_rot_prisma_sup = prisma_sup->agregar(rotate(0.0f, vec3{0.0, 0.0, 1.0}));

    prisma_sup->agregar(translate(vec3{0.0,float(h),0.0}));
    prisma_sup->agregar(scale(vec3{float(w)/2.0,float(h)/2.0,float(w)/2.0}));
    prisma_sup->agregar(translate(vec3{0.0,1.0,0.0}));
    prisma_sup->agregar(new Cubo());

    //prisma compleo
    prismas->agregar(prisma_inf);
    prismas->agregar(prisma_sup);

    pm_tras_inf = prisma_inf->leerPtrMatriz(ind_tras_prisma_inf);
    pm_rot_sup = prisma_sup->leerPtrMatriz(ind_rot_prisma_sup);

    agregar(prismas);

}

unsigned MallaEjercicio3::leerNumParametros() const{
    return 2; //numero de elementos que se mueven de forma independiente
}

void MallaEjercicio3::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    switch(iParam){
        case 0:
            {
                float vmin = 0.0;  // 0 grados en radianes
                float vmax = M_PI/2;   // 90 grados en radianes
                float a_oscilante = (vmax + vmin)/2; // Punto medio = 0
                float b_oscilante = (vmax - vmin)/2; // Amplitud = π/4
                float n = 1.0f; // Frecuencia de oscilación
                *pm_rot_sup = translate(vec3{0.0,3.0,0.0}) * rotate(float(a_oscilante+b_oscilante*sin(M_PI*n*t_sec)), 
                                       vec3{0.0, 0.0, 1.0}) * translate(vec3{-0.0,-3.0,0.0});;
            }
            break;
            
        case 1:
            {
                float vmin = M_PI/8;  // -45 grados en radianes
                float vmax = M_PI/4;   // 45 grados en radianes
                float a_oscilante = (vmax + vmin)/2; // Punto medio = 0
                float b_oscilante = (vmax - vmin)/2; // Amplitud = π/4
                float n = 0.5f; // Frecuencia de oscilación

                *pm_tras_inf = scale(vec3{1.0, a_oscilante + b_oscilante*sin(2*M_PI*n*t_sec), 1.0});
            }
            break;
    }
}