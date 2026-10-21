// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de grafos de escena (implementación). Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de: 
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
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
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


#include "malla-revol.h" //para el cono
using namespace glm;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != nullptr );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const glm::mat4 & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new glm::mat4() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != nullptr );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );

   //cout << "Entra " << __FUNCTION__ << ": " << leerNombre() << endl ;

    // comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *          cauce           = aplicacionIG->cauce ;           assert( cauce != nullptr );
   PilaMateriales * pila_materiales = aplicacionIG->pila_materiales ; assert( pila_materiales != nullptr );

   // COMPLETAR: práctica 3: implementar la visualización del nodo
   //
   // Se deben de recorrer las entradas y llamar recursivamente de visualizarGL, pero 
   // teniendo en cuenta que, al igual que el método visualizarGL de las mallas indexadas,
   // si el nodo tiene un color, debemos de cambiar el color del cauce (y hacer push/pop). 
   // Además, hay que hacer push/pop de la pila de modelado. 
   // Así que debemos de dar estos pasos:
   //
   // 1. Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //     - hacer push del color actual del cauce (con 'pushColor') y después
   //     - fijar el color en el cauce (con 'fijarColor'), usando el color del objeto (se lee con 'leerColor()')
   if(tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   // 2. Guardar copia de la matriz de modelado (con 'pushMM'),
   //guarda modelview actual
   cauce->pushMM();

   // 3. Para cada entrada del vector de entradas:
   //     - si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
   //     - si la entrada es de tipo transformación: componer la matriz (con 'compMM')
   // recorrer todas las entradas del array que hay en el nodo:
   for( unsigned i = 0 ; i < entradas.size() ; i++ ){

      switch( entradas.at(i).tipo ){ 
         case TipoEntNGE::objeto :
            // si es entrada objeto:
            entradas[i].objeto->visualizarGL(); // visualizar objeto
         break ;
         case TipoEntNGE::transformacion :
            // si entrada transf.:
            cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
      }

   }
   
   // 4. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();

   // 5. Si el objeto tiene color asignado:
   //     - restaurar el color original a la entrada (con 'popColor')
   if(tieneColor()){
      cauce->popColor();
   }


   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada    
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)

   // ......


}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL(  )
{
   using namespace std ;
   // comprobar que hay un cauce 
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce; assert( cauce != nullptr );
  
   // COMPLETAR: práctica 3: implementar la visualización del nodo (ignorando colores)
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', pero más simple,
   // Se dan estos pasos:
   //
   // 1. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();

   // 2. Para cada entrada del vector de entradas:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'.
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').
   for( unsigned i = 0 ; i < entradas.size() ; i++ ){

      switch( entradas.at(i).tipo ){ 
         case TipoEntNGE::objeto :
            // si es entrada objeto:
            entradas[i].objeto->visualizarGeomGL(); // visualizar objeto
         break ;
         case TipoEntNGE::transformacion :
            // si entrada transf.:
            cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
      }

   }

   //   3. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();

}

// -----------------------------------------------------------------------------
// Visualizar las normales de los objetos del nodo

void NodoGrafoEscena::visualizarNormalesGL(  )
{
   using namespace std ;

   // comprobar que hay un cauce 
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce; assert( cauce != nullptr );
  

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

   // .......

}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   // 
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1 
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent'). 
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')   
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........


}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   entradas.push_back(entrada);

   return (entradas.size()-1) ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const glm::mat4 & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
glm::mat4 * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: leer un puntero a una matriz en una entrada de un nodo
   //
   // Devolver el puntero a la matriz en la entrada indicada por 'indice'. 
   // Debe de dar error y abortar si: 
   //   - el índice está fuera de rango 
   //   - la entrada no es de tipo transformación
   //   - el puntero a la matriz es nulo 
   glm::mat4 * ptr_matrix;
   if( indice < entradas.size()){
      if(entradas.at(indice).tipo==TipoEntNGE::transformacion ){
         if(entradas.at(indice).matriz != nullptr){
            ptr_matrix = entradas.at(indice).matriz;
         }else{
            std::cerr << "El puntero a la matriz es nulo" << std::endl;
            exit(1);
         }
      }else{
         std::cerr << "La entrada no es de tipo trasformacion" << std::endl;
      }
   }else{
      std::cerr << "El indice esta fuera de rango" << std::endl;
   }


   // Sustituir 'return nullptr' por lo que corresponda.
   //
   return ptr_matrix ;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std ;
   using namespace glm ;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int          ident_busc, // identificador a buscar
   const glm::mat4 &  mmodelado,  // matriz de modelado
   Objeto3D       **  objeto,     // (salida) puntero al puntero al objeto
   glm::vec3 &        centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std ;
   using namespace glm ;
   
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........


   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........


   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}

// *********************************************************************
//Clase GrafoEstrellaX
GrafoEstrellaX::GrafoEstrellaX(unsigned n)
{

   unsigned indice = 1;

   ponerNombre("GrafoEstrellaX");
   NodoGrafoEscena *grafo_estrellaX = new NodoGrafoEscena();
   unsigned ind_rot_estrella = grafo_estrellaX->agregar(rotate(0.0f, vec3{1.0, 0.0, 0.0}));

   //Estrella
   NodoGrafoEscena *estrellaX = new NodoGrafoEscena();
   ponerNombre("estrellaX");
   estrellaX->ponerIdentificador(indice);
   indice++;

   estrellaX->agregar(scale(vec3{2.6, 2.6, 2.6}));
   estrellaX->agregar(translate(vec3{-0.5, -0.5, 0.0})); //Para usar translate hay que añadir using namespace glm;
   estrellaX->agregar(new EstrellaZ(n));

   //Conos
   float radio = 1.3;
   float angulo = (2*M_PI)/n;
   for(unsigned i=0; i<n; i++){

      NodoGrafoEscena *cono = new NodoGrafoEscena();
      cono->ponerIdentificador(indice);
      indice++;

      cono->agregar(translate(vec3{radio*cos(angulo*i),radio*sin(angulo*i), 0.0}));
      cono->agregar(rotate(float(angulo*i), vec3{0.0, 0.0 ,1.0}));
      cono->agregar(rotate(float(-M_PI/2), vec3{0.0, 0.0 ,1.0}));
      cono->agregar(scale(vec3{0.14, 0.15, 0.14}));
      cono->agregar(new Cono(100,50));
      grafo_estrellaX->agregar(cono);

   }
   grafo_estrellaX->agregar(estrellaX);

   agregar(grafo_estrellaX);
   pm_rot_estrella = grafo_estrellaX->leerPtrMatriz(ind_rot_estrella);


}

unsigned GrafoEstrellaX::leerNumParametros() const{
   return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    switch(iParam){
        case 0:
            {
                float a = 0.0;
                float b = (2.0*M_PI)*2.5; //frecuencia = 2.5 ciclos por segundo
                *pm_rot_estrella = rotate(a+(b*t_sec), vec3{1.0, 0.0, 0.0});
            }
            break;
    }
}

// *********************************************************************
//Clase GrafoCubos

GrafoCubos::GrafoCubos()
{

   unsigned indice = 1;

   ponerNombre("grafoCubos");
   NodoGrafoEscena *grafo_cubos = new NodoGrafoEscena();

   NodoGrafoEscena *lado1 = new NodoGrafoEscena();
   lado1->ponerIdentificador(indice);
   indice++;
   lado1->agregar(translate(vec3(-0.5,-0.5,-0.5)));
   lado1->agregar(new LadoGrafoCubos(pm_rot_cubo1));

   NodoGrafoEscena *lado2 = new NodoGrafoEscena();
   lado2->ponerIdentificador(indice);
   indice++;
   lado2->agregar(translate(vec3(-0.5,0.5,-0.5)));
   lado2->agregar(rotate(-(float)M_PI/2,vec3{0,0,1}));
   lado2->agregar(new LadoGrafoCubos(pm_rot_cubo2));

   NodoGrafoEscena *lado3 = new NodoGrafoEscena();
   lado3->ponerIdentificador(indice);
   indice++;
   lado3->agregar(translate(vec3(-0.5,0.5,0.5)));
   lado3->agregar(rotate((float)M_PI,vec3{1,0,0}));
   lado3->agregar(new LadoGrafoCubos(pm_rot_cubo3));

   NodoGrafoEscena *lado4 = new NodoGrafoEscena();
   lado4->ponerIdentificador(indice);
   indice++;
   lado4->agregar(translate(vec3(0.5,-0.5,-0.5)));
   lado4->agregar(rotate((float)M_PI/2,vec3{0,0,1}));
   lado4->agregar(new LadoGrafoCubos(pm_rot_cubo4));

   NodoGrafoEscena *lado5 = new NodoGrafoEscena();
   lado5->ponerIdentificador(indice);
   indice++;
   lado5->agregar(translate(vec3(0.5,-0.5,-0.5)));
   lado5->agregar(rotate((float)M_PI,vec3{0,1,1}));
   lado5->agregar(new LadoGrafoCubos(pm_rot_cubo5));

   NodoGrafoEscena *lado6 = new NodoGrafoEscena();
   lado6->ponerIdentificador(indice);
   indice++;
   lado6->agregar(translate(vec3(0.5,-0.5,0.5)));
   lado6->agregar(rotate((float)M_PI/2,vec3{0,0,1}));
   lado6->agregar(rotate(3*(float)M_PI/2,vec3{1,0,0}));
   lado6->agregar(new LadoGrafoCubos(pm_rot_cubo6));

   grafo_cubos->agregar(lado1);
   grafo_cubos->agregar(lado2);
   grafo_cubos->agregar(lado3);
   grafo_cubos->agregar(lado4);
   grafo_cubos->agregar(lado5);
   grafo_cubos->agregar(lado6);

   agregar(grafo_cubos);

}

unsigned GrafoCubos::leerNumParametros() const{
   return 1;
}

void GrafoCubos::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    switch(iParam){
        case 0:
            {
                float a = 0.0;
                float b = (2.0*M_PI); //frecuencia = 1 ciclos por segundo
                *pm_rot_cubo1 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_rot_cubo2 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_rot_cubo3 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_rot_cubo4 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_rot_cubo5 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
                *pm_rot_cubo6 = rotate(a+(b*t_sec), vec3{0.0, 1.0, 0.0});
            }
            break;
    }
}

LadoGrafoCubos::LadoGrafoCubos(mat4 * &pm_rot){

   NodoGrafoEscena  *lado = new NodoGrafoEscena();
   lado->agregar(new RejillaY(7,7));
   agregar(lado);

   NodoGrafoEscena * paralelepipedo = new NodoGrafoEscena();
   paralelepipedo->agregar(translate(vec3(0.5,-0.25,0.5)));
   paralelepipedo->agregar(scale(vec3(0.125,0.25,0.125)));
   unsigned ind = paralelepipedo->agregar(rotate(0.0f,vec3{0.0,1.0,0.0}));
   paralelepipedo->agregar(new Cubo());
   agregar(paralelepipedo);
   pm_rot = paralelepipedo->leerPtrMatriz(ind);

}






