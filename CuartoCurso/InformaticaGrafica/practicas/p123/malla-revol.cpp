// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas creadas por revolución de un perfil (implementación). Clase 'MallaRevol' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **    + MallaRevol: malla indexada de triángulos obtenida por 
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida 
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   using namespace glm ;
   
   // COMPLETAR: práctica 2: implementar algoritmo de creación de malla de revolución
   //
   // Escribir el algoritmo de creación de una malla indexada por revolución de un 
   // perfil, según se describe en el guion de prácticas.
   

   //Creamos los vertices
   for(unsigned i=0; i<num_copias; i++){
      for(unsigned j=0; j<perfil.size(); j++){
         float tita = (2*M_PI*i)/(num_copias-1);

         //matriz de giro con respecto al eje y
         vector<vector<float>> turn_matrix_y= 
         {  {cos(tita), 0.0, sin(tita)}, 
            {0.0, 1.0, 0.0},
            {-sin(tita), 0.0, cos(tita)},
         };

         //Nuevo vertice es turn_matrix_y*perfil[j]
         glm::vec3 q = {turn_matrix_y.at(0).at(0)*perfil.at(j).x + turn_matrix_y.at(0).at(1)*perfil.at(j).y + turn_matrix_y.at(0).at(2)*perfil.at(j).z,
                      turn_matrix_y.at(1).at(0)*perfil.at(j).x + turn_matrix_y.at(1).at(1)*perfil.at(j).y + turn_matrix_y.at(1).at(2)*perfil.at(j).z,
                      turn_matrix_y.at(2).at(0)*perfil.at(j).x + turn_matrix_y.at(2).at(1)*perfil.at(j).y + turn_matrix_y.at(2).at(2)*perfil.at(j).z,  
                     };
         
         vertices.push_back(q);

      }
   }

   //Creamos los triangulos
   for(unsigned i=0; i<num_copias-1; i++){
      for(unsigned j=0; j<perfil.size()-1; j++){

            int k = i*perfil.size() + j;
            triangulos.push_back({k, k+perfil.size(), k+perfil.size()+1});
            triangulos.push_back({k, k+perfil.size()+1, k+1});

      }
   }







}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   vector<glm::vec3> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);

}

// -----------------------------------------------------------------------------

Cilindro::Cilindro
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
){
   
   ponerNombre("Cilindro");
   
   std::vector<glm::vec3> perfil = vector<glm::vec3>();
   for(int i=0; i<num_verts_per; i++){
      float distancia_entre_puntos = i*(1.0/(num_verts_per-1)); //vec3 son de tipo float
      glm::vec3 aux = {1.0,distancia_entre_puntos,0.0};
      perfil.push_back(aux);
   }

   inicializar(perfil, nperfiles);

}

// -----------------------------------------------------------------------------

Cono::Cono
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
){

   ponerNombre("Cono");

   std::vector<glm::vec3> perfil;
   for(int i=0; i<num_verts_per; i++){
      float y = i*(1.0/(num_verts_per-1));
      double x = y - 1;
      glm::vec3 aux = {x,y,0.0};
      perfil.push_back(aux);
   }

   inicializar(perfil, nperfiles);

}

// -----------------------------------------------------------------------------

Esfera::Esfera
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
){
   ponerNombre("Esfera");

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

// -----------------------------------------------------------------------------




