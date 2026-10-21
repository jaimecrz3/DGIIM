// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#include "examen-ec-p45.h"
#include "aplicacion-ig.h"
#include "seleccion.h"
#include "camara.h"

//Ejercicio 1

PiramideEx::PiramideEx(){
   vertices = {

    //lateral dcha
      {1.0, 0.0, 1.0},
      {1.0, 0.0, -1.0},
      {0.0, 2.0, 0.0},

    //lateral izq
      {-1.0, 0.0, -1.0},
      {-1.0, 0.0, 1.0},
      {0.0, 2.0, 0.0},

    //frontal
    {-1.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {0.0, 2.0, 0.0},

    //trasera
    {1.0, 0.0, -1.0},
    {-1.0, 0.0, -1.0},
    {0.0, 2.0, 0.0},

    //base
    {-1.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, -1.0},
    {-1.0, 0.0, -1.0},
    


   };

   triangulos = {
      {0, 1, 2},
      {3, 4, 5},
      
      {6, 7, 8},
      {9, 10, 11},

      {12, 13, 14},
      {12, 14, 15},
   };

   cc_tt_ver = {

    {0.0, 1.0},
    {1.0, 1.0},
    {0.5, 0.0},

    {0.0, 1.0},
    {1.0, 1.0},
    {0.5, 0.0},

    {0.0, 1.0},
    {1.0, 1.0},
    {0.5, 0.0},

    {0.0, 1.0},
    {1.0, 1.0},
    {0.5, 0.0},

    {0.0, 1.0},
    {1.0, 1.0},
    {1.0, 0.0},
    {0.0, 0.0},


   };

    calcularNormales();

}

PiramideUGR::PiramideUGR(){
   Textura *textura = new Textura("window-icon.jpg");
   Material *material = new Material(textura, 0.7, 0.5, 0.0, 30.0);

   ponerNombre("Piramide UGR");
   agregar(material);
   agregar(new PiramideEx());
}

//Ejercicio 2
TrianguloEx::TrianguloEx(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){

    //ponerNombre("Triangulo: " + n);
    glm::vec3 color = {1.0, 1.0, 1.0};
   ponerColor(color);

   vertices.push_back(v1);
   vertices.push_back(v2);
   vertices.push_back(v3);

   triangulos = {
      {0, 1, 2},
   };

    calcularNormales();

    glm::vec3 centro;
    centro.x = (v1.x + v2.x + v3.x)/3;
    centro.y = (v1.y + v2.y + v3.y)/3;
    centro.z = (v1.z + v2.z + v3.z)/3;
    ponerCentroOC(centro);

}

bool TrianguloEx::cuandoClick(const glm::vec3 & centro_wc){

    using namespace std ;
   assert( aplicacionIG != nullptr );
   Escena * escena = aplicacionIG->escenas[aplicacionIG->ind_escena_act] ; assert( escena != nullptr );

   cout << "Click realizado sobre el objeto: '" << leerNombre() << "'" << endl ;
   escena->camaraActual()->mirarHacia(centro_wc);

   if(leerColor() == glm::vec3(1.0,1.0,1.0)){
      ponerColor(glm::vec3(1.0,0.0,0.0));
   }else{
      ponerColor(glm::vec3(1.0,1.0,1.0));
   }

   return true;

}

PiramideTriangulos::PiramideTriangulos(){
    ponerNombre("Ejercicio 2 exp45");

    unsigned id=1;

    //Lateral dcha
   TrianguloEx *triangulo0 = new TrianguloEx({1.0, 0.0, 1.0}, {1.0, 0.0, -1.0},{0.0, 2.0, 0.0});
   triangulo0->ponerNombre("Triangulo 0");
    triangulo0->ponerIdentificador(id);
    id++;
   //Lateral izqd
    TrianguloEx *triangulo1 = new TrianguloEx({-1.0, 0.0, -1.0},{-1.0, 0.0, 1.0},{0.0, 2.0, 0.0});
    triangulo1->ponerNombre("Triangulo 1");
    triangulo1->ponerIdentificador(id);
    id++;
   //Frontal
    TrianguloEx *triangulo2 = new TrianguloEx({-1.0, 0.0, 1.0},{1.0, 0.0, 1.0},{0.0, 2.0, 0.0});
    triangulo2->ponerNombre("Triangulo 2");
    triangulo2->ponerIdentificador(id);
    id++;
   //trasera
   TrianguloEx *triangulo3 = new TrianguloEx({1.0, 0.0, -1.0},{-1.0, 0.0, -1.0}, {0.0, 2.0, 0.0});
   triangulo3->ponerNombre("Triangulo 3");
    triangulo3->ponerIdentificador(id);
    id++;

   //base
   TrianguloEx *triangulo4 = new TrianguloEx({-1.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, 0.0, -1.0});
   triangulo4->ponerNombre("Triangulo 4");
    triangulo4->ponerIdentificador(id);
    id++;

    TrianguloEx *triangulo5 = new TrianguloEx({-1.0, 0.0, 1.0},{1.0, 0.0, -1.0},{-1.0, 0.0, -1.0});
    triangulo5->ponerNombre("Triangulo 5");
    triangulo5->ponerIdentificador(id);
    id++;

    agregar(triangulo0);
    agregar(triangulo1);
    agregar(triangulo2);
    agregar(triangulo3);
    agregar(triangulo4);
    agregar(triangulo5);

}
