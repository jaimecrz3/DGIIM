// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#ifndef EXAMEN_EC_P123_HPP
#define EXAMEN_EC_P123_HPP
#include "malla-ind.h"
#include "grafo-escena.h"

// ---------------------------------------------------------------------

class MallaEjercicio1 : public MallaInd
{
   public:
      MallaEjercicio1();
};

// ---------------------------------------------------------------------

class MallaEjercicio2 : public MallaInd
{
   public:
      MallaEjercicio2(unsigned n);
};

// ---------------------------------------------------------------------

class MallaEjercicio3 : public NodoGrafoEscena
{
    protected:
        mat4 *pm_rot_sup = nullptr;
        mat4 *pm_tras_inf = nullptr;

    public:
        unsigned leerNumParametros() const;
        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

        MallaEjercicio3(unsigned w, unsigned h);

};

#endif
