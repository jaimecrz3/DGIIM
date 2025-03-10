// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#ifndef LATA_PEONES_HPP
#define LATA_PEONES_HPP

#include <grafo-escena.h>

using namespace std;

// ---------------------------------------------------------------------
//Clase Lata
class Lata : public NodoGrafoEscena{
    public:
        Lata(const std::string & nombreArchivoJPG);
};

// ---------------------------------------------------------------------
//Clase Peon
class Peon : public NodoGrafoEscena{
    public:
        Peon(unsigned nPerfiles);
        bool cuandoClick( const glm::vec3 & centro_wc) override;

    private:
        glm::mat4* pm_tras = nullptr;
};

// ---------------------------------------------------------------------
//Clase LataPeones
class LataPeones : public NodoGrafoEscena{
    public:
        LataPeones();
};

// ---------------------------------------------------------------------
//Clase VariasLatasPeones
class VariasLatasPeones : public LataPeones{
    public:
        VariasLatasPeones();
};

#endif