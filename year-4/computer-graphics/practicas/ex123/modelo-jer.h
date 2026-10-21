// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Objeto derivado de NodoGrafoEscena: Submarino (declaraciones).  Clase 'Submarino' y relacionadas.
// ** Jaime Corzo Galdó
// ** email: jcggranada04@gmail.com, DNI: 77559045D
// **
// ** Declaraciones de: 
// **     + Clase 'Submarino' (derivada de 'NodoGrafoEscena')
// **     + Clase 'Esferas' (para el cuerpo del submarino y el espejo del periscopio)
// **     + Clase 'Cilindro' (para el palo del periscopio)
// **     + Clase 'EstrellaZ' (para la helice)
// **
// **
// **
// *********************************************************************

#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP
 

#include "grafo-escena.h"
#include "malla-revol.h"
#include "malla-ind.h"


using namespace std;
using namespace glm;


// *********************************************************************
// Clase esferas
class Esferas : public MallaRevol
{
    public:
    Esferas
    ( 
        const int num_verts_per, // número de vértices del perfil original (M )
        const unsigned nperfiles // número de perfiles (N )
    ) ;

};


// *********************************************************************
// Clase CilindroPeriscopio
class CilindroPeriscopio : public MallaRevol
{
    public:
    CilindroPeriscopio
    (
        const int num_verts_per, // número de vértices del perfil original (m)
        const unsigned nperfiles // número de perfiles (n)
    );
};

// *********************************************************************
// Clase EstrellaZHelice
class EstrellaZHelice : public MallaInd
{
    public:
    EstrellaZHelice();
};

// *********************************************************************
// Clase CajaPeriscopio
class CajaPeriscopio : public MallaInd
{
   public:
      CajaPeriscopio();
};

// *********************************************************************
// Clase TrianguloAla
class TrianguloAla : public MallaInd
{
   public:
      TrianguloAla();
};

// *********************************************************************
// Clase CuadradoAla
class CuadradoAla : public MallaInd
{
   public:
      CuadradoAla();
};

// *********************************************************************
// Clase Submarino
class Submarino : public NodoGrafoEscena
{
    protected:
        mat4 *pm_rot_espejo = nullptr;
        mat4 *pm_rot_helice = nullptr;
        mat4 *pm_rot_cuerpo = nullptr;
        mat4 *pm_tras_cuerpo = nullptr;
        mat4 *pm_tras_ala1 = nullptr;
        mat4 *pm_tras_ala2 = nullptr;

    public:
        unsigned leerNumParametros() const;
        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

        Submarino();

};


#endif // MODELO_JER_HPP
