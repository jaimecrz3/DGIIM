// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación). Clase 'MallaInd' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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
#include "aplicacion-ig.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   
   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   for(unsigned i=0; i<nt; i++){

      //n cada cara se consideran las posiciones (coordenadas de objeto) de sus tres vértices
      glm::vec3 p = vertices.at(triangulos.at(i).x);
      glm::vec3 q = vertices.at(triangulos.at(i).y);
      glm::vec3 r = vertices.at(triangulos.at(i).z);

      //A partir de estas coordenadas se calculan los vectores a y b correspondientes a dos aristas
      glm::vec3 a = q-p;
      glm::vec3 b = r-p;

      //El vector mc , perpendicular a la cara, se obtiene como el producto vectorial de las aristas
      glm::vec3 m_c = cross(a,b);
      //el vector normal nc (de longitud unidad) se obtiene normalizando mc , esto es: nc = mc /∥mc ∥.
      if(length(m_c)!=0.0){
         m_c = normalize(m_c);
      }else{
         m_c = {0.0,0.0, 0.0};
      }

      nor_tri.push_back(m_c);
   
   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm ;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   calcularNormalesTriangulos();

   //Para cada vértice, el vector mv , es un vector aproximadamente perpendicular a la superficie 
   //de la malla en la posición del vértice.
   //Hacerlo O(n)
   nor_ver = std::vector<vec3>(vertices.size(), {0.0,0.0,0.0});
   for(unsigned i=0; i<triangulos.size(); i++){
      unsigned indice_p = triangulos.at(i).x;
      nor_ver.at(indice_p) = nor_ver.at(indice_p) + nor_tri.at(i);

      unsigned indice_q = triangulos.at(i).y;
      nor_ver.at(indice_q) = nor_ver.at(indice_q) + nor_tri.at(i);

      unsigned indice_r = triangulos.at(i).z;
      nor_ver.at(indice_r) = nor_ver.at(indice_r) + nor_tri.at(i);

   }

   for(unsigned i=0; i<nor_ver.size(); i++){
      if(length(nor_ver.at(i))!=0.0){
         nor_ver.at(i) = normalize(nor_ver.at(i));
      }
   }


}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( )
{
   // comprobar algunas precondiciones básicas
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );
   CError();

   // si la malla no vértices o no tiene triángulos, imprimir advertencia y salir.
   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // COMPLETAR: práctica 1: cambiar color del cauce
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer push del color actual del cauce
   //    - fijar el color en el cauce usando el color del objeto (se lee con 'leerColor()')
   if(this->tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   // COMPLETAR: práctica 1: crear el descriptor de VAO, si no está creado
   //  Si el puntero 'dvao' es nulo, crear el descriptor de VAO
   //   * en primer lugar se crea el descriptor de VAO, con su constructor 
   //     (se le pasa como parámetro la tabla de posiciones y el número de atributos del cauce). 
   //   * se añade el descriptor de VBO con la tabla de índices (la tabla de triángulos),
   //   * finalmente se añaden al VAO los descriptores VBOs con tablas de atributos 
   //     que no estén vacías
   //  Si el VAO ya está creado, (dvao no nulo), no hay que hacer nada.
   //
   if(dvao == nullptr){
      dvao = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones,vertices));
      dvao->agregar(new DescrVBOInds(triangulos));

      if(size(col_ver) > 0){
         dvao->agregar(new DescrVBOAtribs(ind_atrib_colores, col_ver));
      }
      if(size(nor_ver) > 0){
         dvao->agregar(new DescrVBOAtribs(ind_atrib_normales,nor_ver));
      }
      if(size(cc_tt_ver) > 0){
         dvao->agregar(new DescrVBOAtribs(ind_atrib_coord_text,cc_tt_ver));
      }

   }

   // COMPLETAR: práctica 1: visualizar el VAO usando el método 'draw' de 'DescrVAO'

   dvao->draw(GL_TRIANGLES);

   // COMPLETAR: práctica 1: restaurar color anterior del cauce 
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer 'pop' del color actual del cauce

   if(this->tieneColor()){
      cauce->popColor();
   }

}


// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
//
//esté método únicamente puede llamarse después de visualizarGL (eso está garantizado 
//ya únicamente se dibujan las aristas después de haber visualizado el objeto con visualizarGL).
//Se llama desde la clase escena en el metodo visualizarGL, en el caso de que el modo de visualizacion
//sea relleno y que el bool aplicacionIG->dibujar_aristas = true(estas dos cosas se cambian con la
//tecla M y W respectivamente)
      
void MallaInd::visualizarGeomGL( )
{
   // Comprobar que el descriptor de VAO ya está creado
   // (es decir, este método únicamente se podrá invocar después de que 
   // se haya llamado a 'visualizaGL')
   
   assert( dvao != nullptr );

   // COMPLETAR: práctica 1: visualizar únicamente la geometría del objeto 
   // 
   //    1. Desactivar todas las tablas de atributos del VAO (que no estén vacías)
   if(size(col_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_colores,0);
   }
   if(size(nor_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_normales,0);
   }
   if(size(cc_tt_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_coord_text,0);
   }

   //    2. Dibujar la malla (únicamente visualizará los triángulos), se usa el método 'draw' del VAO (dvao)
   dvao->draw(GL_TRIANGLES);

   //    3. Volver a activar todos los atributos para los cuales la tabla no esté vacía
   if(size(col_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_colores,1);
   }
   if(size(nor_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_normales,1);
   }
   if(size(cc_tt_ver) > 0){
      dvao->habilitarAtrib(ind_atrib_coord_text,1);
   }

}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime 
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  

   if( nor_ver.size() != vertices.size() )
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl ;
      cout << "Nombre del objeto        : " << leerNombre() << endl ;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl ;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl ;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   // 
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo, 
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector 
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona 
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se 
   //          necesitan).
   // 
   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el 
   //       tipo de primitiva 'GL_LINES'.
   if(dvao_normales==nullptr){
      auto it_normales = nor_ver.begin();
      for(auto it=vertices.begin(); it!=vertices.end(); ++it){
         segmentos_normales.push_back(*it);
         segmentos_normales.push_back(*it + (0.35f*(*it_normales)));
         ++it_normales;
      }

      dvao_normales = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones,segmentos_normales));
      
      //Puesto que la creación del VAO supone hacer una copia de los datos, la tabla segmentos_normales se puede vaciar después
      //de haber creado el VAO.
      segmentos_normales.clear();

   }

   dvao_normales->draw(GL_LINES);

}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL() 
{

   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar la malla en modo selección 
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color 
   // obtenido a partir del identificador. El código da estos pasos:
   // 
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene 
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a 
   //         partir del identificador (con 'ColorDesdeIdent'). 
   int id = leerIdentificador();
   if(id!=-1){
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(id));
   }

   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   visualizarGeomGL();

   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   if(id!=-1){
      cauce->popColor();
   }

}


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   LeerPLY(nombre_arch, vertices, triangulos);


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   calcularNormales();

}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
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

      calcularNormales();

}

// ****************************************************************************
//Clase Tetaedro

Tetaedro::Tetaedro()
:  MallaInd( "tetaedro 4 vertices" )
{

   vertices =
   {  { -1.0, -1.0, -1.0 }, // 0
      { +1.0, -1.0, -1.0 }, // 1
      { +0.0, +1.0, -1.0 }, // 2
      { +0.0, +0.0, +1.0 }, // 3
   };

   triangulos =
   {  {0,1,2},
      {0,1,3},
      {1,2,3},
      {0,2,3}

   };

   glm::vec3 color_tetaedro;
   //negro: color_tetaedro = {0.0, 0.0, 0.0};
   //blanco: color_tetaedro = {1.0, 1.0, 1.0};
   //rojo: color_tetaedro = {1.0, 0.0, 0.0};
   //verde: color_tetaedro = {0.0, 1.0, 0.0};
   //azul: color_tetaedro = {0.0, 0.0, 1.0};
   //amarillo: color_tetaedro = {1.0, 1.0, 0.0};
   color_tetaedro = {0.6786, 0.123, 0.893};
   ponerColor(color_tetaedro);

   calcularNormales();

}

// ****************************************************************************
// Clase CuboColores

CuboColores::CuboColores()
:  MallaInd( "cubo de colores 8 vértices" )
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

      col_ver =
      {  {0.0, 0.0, 0.0},
         {0.0, 0.0, 1.0},
         {0.0, 1.0, 0.0},
         {0.0, 1.0, 1.0},
         {1.0, 0.0, 0.0},
         {1.0, 0.0, 1.0},
         {1.0, 1.0, 0.0},
         {1.0, 1.0, 1.0},
      };

}

// ****************************************************************************
// Clase EstrellaZ

//El centro es (0.5,0.5,0)
//Podemos ver que la estrella tiene vertices en una circunferencia de radio=0.5
//Luego los vertices que estan en la circunferencia son de la forma: (con i en {0, 2*n-1})
//x_i = x_centro + radio*cos(angulo*i)
//y_i = y_centro + radio*sen(angulo*i)
//z_i = 0
//Donde angulo = (2*pi)/n
//
//Para los vertices que no estan en la circunferencia externa, hay que tomar el mismo angulo pero con radio la mitad
//
EstrellaZ::EstrellaZ(unsigned n)
:  MallaInd( "Estrella plana en el plano perpendicular al eje z con n puntas" )
{

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

   for(unsigned i=0; i<2*n; i++){
      col_ver.push_back(vertices[i]);
   }
   col_ver.push_back({1.0,1.0,1.0});   
   

}

// ****************************************************************************
// Clase CasaX

CasaX::CasaX()
:  MallaInd( "CasaX" )
{

   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +0.75 }, // 1
         { 0.0, 0.75, 0.0 }, // 2
         { 0.0, 0.75, +0.75 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +0.75 }, // 5
         { +1.0, 0.75, 0.0 }, // 6
         { +1.0, 0.75, +0.75 }, // 7
         { 0.0, 1.0, 0.375 }, // 8
         { 1.0, 1.0, 0.375}, // 9
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3},  // Z+ (+1)

         {6,7,9}, {2,3,8},
         {3,7,9}, {3,8,9},
         {2,8,9},{2,6,9}
      } ;

      for(unsigned i=0; i<vertices.size(); i++){
         col_ver.push_back(vertices[i]);
      }

}

// ****************************************************************************
// Clase MallaTriangulo

MallaTriangulo::MallaTriangulo()
:  MallaInd( "MallaTriangulo" )
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

MallaCuadrado::MallaCuadrado()
:  MallaInd( "MallaCuadrado" )
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

// ****************************************************************************
// Clase MallaPiramideL

MallaPiramideL::MallaPiramideL()
:  MallaInd( "MallaPiramideL" )
{

   vertices =
      {  { 0, 0, 2 }, 
         { 0, 0, 0 },  
         { 1, 0, 2 }, 
         { 1, 0, 1 }, 
         { 2, 0, 1 },  
         { 2, 0, 0}, 
         { 0, 3, 0}, 
      } ;



   triangulos =
      {  {0,2,3},
         {0,1,5}, 
         {3,4,5},
         {3,6,4},
         {1,6,5},
         {1,6,0},
         {2,3,6},
         {0,2,6},
         {4,5,6}
      } ;

}

// ****************************************************************************
// Clase PiramideEstrellaZ

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
:  MallaInd( "PiramideEstrellaZ" )
{

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
   //nuevo vertice
   vertices.push_back({0.5, 0.5, 0.5});

   for(unsigned i=0; i<2*n; i++){
      triangulos.push_back({i,(i+1)%(2*n), 2*n}); //El modulo 2*n solo es par el ultimo caso cuando i+1=2*n
      triangulos.push_back({i, (i+1)%(2*n), 2*n+1});
   }

   for(unsigned i=0; i<2*n; i++){
      col_ver.push_back(vertices[i]);
   }
   col_ver.push_back({1.0,1.0,1.0}); 
   col_ver.push_back({1.0,1.0,1.0});

}

// ****************************************************************************
// Clase RejillaY

RejillaY::RejillaY(unsigned n,unsigned m)
:  MallaInd( "RejillaY" )
{

   assert(n > 1 && m > 1);
   float escala_n = 1.0f/(n-1);
   float escala_m = 1.0f/(m-1);
   for(unsigned i=0; i<n; i++){
      for(unsigned j=0; j<m; j++){
         vertices.push_back({i*escala_n,0.0f,j*escala_m});
      }
   }
   for(unsigned i=0; i<n-1; i++){
      for(unsigned j=0; j<m-1; j++){
         triangulos.push_back({i*m+j,i*m+j+1,(i+1)*m+j});
         triangulos.push_back({i*m+j+1,(i+1)*m+j+1,(i+1)*m+j});
      }
   }
   for(unsigned i=0;i<vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }

}

// ****************************************************************************
// Clase RejillaY

MallaTorre::MallaTorre(unsigned n)
:  MallaInd( "MallaTorre" )
{
   
   assert(n > 1);
   unsigned int j = 0;

   //base torre
   vertices.push_back({+0.5,0,+0.5});
   vertices.push_back({-0.5,0,+0.5});
   vertices.push_back({-0.5,0,-0.5});
   vertices.push_back({+0.5,0,-0.5});


   for(unsigned i = 1; i <= n; ++i)
   {
      j += 4;
      vertices.push_back({+0.5,i,+0.5});
      vertices.push_back({-0.5,i,+0.5});
      vertices.push_back({-0.5,i,-0.5});
      vertices.push_back({+0.5,i,-0.5});

      triangulos.push_back({j-4, j-3, j});
      triangulos.push_back({j-3, j+1, j});

      triangulos.push_back({j-3, j-2, j+1});
      triangulos.push_back({j-2, j+2, j+1});

      triangulos.push_back({j-2, j-1, j+2});
      triangulos.push_back({j-1, j+3, j+2});

      triangulos.push_back({j-1, j-4, j+3});
      triangulos.push_back({j-4, j, j+3});
   }

}

// ****************************************************************************
// Clase Cubo24

Cubo24::Cubo24() : MallaInd("Cubo24")
{

   vertices =
      {
         {+1.0, -1.0, +1.0}, // 0
         {+1.0, +1.0, +1.0}, // 1
         {+1.0, +1.0, -1.0}, // 2
         {+1.0, -1.0, -1.0}, // 3


         {-1.0, -1.0, +1.0}, // 4
         {-1.0, +1.0, +1.0}, // 5
         {+1.0, +1.0, +1.0}, // 6
         {+1.0, -1.0, +1.0}, // 7


         {-1.0, -1.0, -1.0}, // 8
         {-1.0, +1.0, -1.0}, // 9
         {-1.0, +1.0, +1.0}, // 10
         {-1.0, -1.0, +1.0}, // 11

         {+1.0, -1.0, -1.0}, // 12
         {+1.0, +1.0, -1.0}, // 13
         {-1.0, +1.0, -1.0}, // 14
         {-1.0, -1.0, -1.0}, // 15


         {+1.0, +1.0, +1.0}, // 16
         {-1.0, +1.0, +1.0}, // 17
         {-1.0, +1.0, -1.0}, // 18
         {+1.0, +1.0, -1.0}, // 19

         {+1.0, -1.0, +1.0}, // 20
         {-1.0, -1.0, +1.0}, // 21
         {-1.0, -1.0, -1.0}, // 22
         {+1.0, -1.0, -1.0}  // 23
      };

   triangulos =
      {
         {0, 3, 2},
         {0, 2, 1},

         {4, 7, 6},
         {4, 6, 5},

         {8, 11, 10},
         {8, 10, 9},

         {12, 15, 14},
         {12, 14, 13},

         {16, 19, 18},
         {16, 18, 17},

         {20, 22, 23},
         {20, 21, 22}
      };
      
    for (int i=0; i < 6; i++){

      // cc_tt_ver.push_back({0.0, 1.0});
      // cc_tt_ver.push_back({0.0, 0.0});
      // cc_tt_ver.push_back({1.0, 0.0});
      // cc_tt_ver.push_back({1.0, 1.0});

      cc_tt_ver.push_back({0.0, 0.0});
      cc_tt_ver.push_back({0.0, 1.0});
      cc_tt_ver.push_back({1.0, 1.0});
      cc_tt_ver.push_back({1.0, 0.0});
   
    }

   calcularNormales();
}

// ****************************************************************************
// Clase MallaDiscoP4

MallaDiscoP4::MallaDiscoP4() : MallaInd("MallaDiscoP4")
{

   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;

   for( unsigned i= 0 ; i < ni ; i++ )
   for( unsigned j= 0 ; j < nj ; j++ )
   {
      const float
         fi = float(i)/float(ni-1),
         fj = float(j)/float(nj-1),
         ai = 2.0*M_PI*fi,
         x = fj * cos( ai ),
         y = fj * sin( ai ),
         z = 0.0 ;
      vertices.push_back({ x, y, z });

      //Ejercicio1
      cc_tt_ver.push_back({float(0.5*x), float(0.5*y)});

      //Ejercicio2
      //cc_tt_ver.push_back({fj, fi});
   }
   for( unsigned i= 0 ; i < ni-1 ; i++ )
   for( unsigned j= 0 ; j < nj-1 ; j++ )
   {
      triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
      triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
   }

}


Piramide::Piramide(){
   vertices = {
      {1.0, 0.0, 1.0},
      {1.0, 0.0, -1.0},
      {-1.0, 0.0, -1.0},
      {-1.0, 0.0, 1.0},
      {0.0, 1.0, 0.0},

      {1.0, 0.0, 1.0},
      {1.0, 0.0, -1.0},
      {-1.0, 0.0, -1.0},
      {-1.0, 0.0, 1.0},
      {0.0, 1.0, 0.0},
   };

   triangulos = {
      {0, 1, 4},
      {2, 3, 4},
      
      {8, 5, 9},
      {6, 7, 9},
   };

   // Piramide rayada en vertical

   // cc_tt_ver = {
   //    {0.0, 0.0},
   //    {1.0, 0.0},
   //    {1.0, 1.0},
   //    {0.0, 1.0},
   //    {0.5, 0.5},

   //    {1.0, 0.0},
   //    {1.0, 1.0},
   //    {0.0, 1.0},
   //    {0.0, 0.0},
   //    {0.5, 0.5},
   // };

   // Piramide rayada en horizontal

   cc_tt_ver = {
      // {0.0, 0.0},
      // {0.0, 1.0},
      // {1.0, 1.0},
      // {1.0, 0.0},
      // {0.5, 0.5},
      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},
      {0.5, 0.5},

      // {0.0,1.0},
      // {1.0,1.0},
      // {1.0,0.0},
      // {0.0,0.0},
      // {0.5,0.5},
      {0.0,0.0},
      {1.0,0.0},
      {1.0,1.0},
      {0.0,1.0},
      {0.5,0.5},
   };

   calcularNormales();
}

// ****************************************************************************  

MallaEXP4::MallaEXP4(){
   vertices = {
      {cos( (2.0*M_PI) / 3.0 ), 0.0, -sin( (2.0*M_PI) / 3.0 )},
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {cos( (2.0*M_PI) / 3.0 ), 0.0, sin( (2.0*M_PI) / 3.0 )},

      {cos( (2.0*M_PI) / 3.0 ), 0.0, -sin( (2.0*M_PI) / 3.0 )},
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {cos( (2.0*M_PI) / 3.0 ), 0.0, sin( (2.0*M_PI) / 3.0 )},

      {cos( (2.0*M_PI) / 3.0 ), 0.0, -sin( (2.0*M_PI) / 3.0 )},
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {cos( (2.0*M_PI) / 3.0 ), 0.0, sin( (2.0*M_PI) / 3.0 )},
   };

   triangulos = {
      {0, 2, 1},
      {4, 5, 3},
      {6, 8, 7},
      {9, 10, 11},
   };

   // Rayas hacia arriba

   cc_tt_ver = {
      {1.0, 1.0-0.0},
      {0.0, 1.0-0.0},
      {0.5, 1.0-1.0},

      {0.0, 1.0-0.0},
      {0.5, 1.0-1.0},
      {1.0, 1.0-0.0},

      {0.5, 1.0-1.0},
      {1.0, 1.0-0.0},
      {0.0, 1.0-0.0},

      {1.0, 1.0-0.0},
      {0.5, 1.0-1.0},
      {0.0, 1.0-0.0},
   };

   // Rayas hacia el lado

   // cc_tt_ver = {
   //    {0.0, 0.0},
   //    {0.0, 1.0},
   //    {1.0, 0.5},

   //    {1.0, 0.5},
   //    {0.0, 0.0},
   //    {0.0, 1.0},

   //    {1.0, 0.5},
   //    {0.0, 1.0},
   //    {0.0, 0.0},

   //    {0.0, 0.0},
   //    {1.0, 0.5},
   //    {0.0, 1.0},
   // };

   calcularNormales();
}

// -----------------------------------------------------------------------------------------------

