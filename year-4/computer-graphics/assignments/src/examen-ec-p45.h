// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#ifndef EXAMEN_EC_P45_HPP
#define EXAMEN_EC_P45_HPP
#include "malla-ind.h"
#include "grafo-escena.h"

// ---------------------------------------------------------------------
//Ejercicio 1
class PiramideEx : public MallaInd
{
   public:
      PiramideEx();
};

class PiramideUGR : public NodoGrafoEscena {
   public:
      PiramideUGR();
};
// ---------------------------------------------------------------------
//Ejercicio 2
class TrianguloEx : public MallaInd
{
   public:
      TrianguloEx(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
       bool cuandoClick(const glm::vec3 & centro_wc) override;
};

class PiramideTriangulos : public NodoGrafoEscena {
   public:
      PiramideTriangulos();
};




#endif