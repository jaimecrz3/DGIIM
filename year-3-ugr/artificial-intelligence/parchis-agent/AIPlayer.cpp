# include "AIPlayer.h"
# include "../../include/model/Parchis.h"

#include "algorithm"

const float masinf = 9999999999.0, menosinf = -9999999999.0;
const float gana = masinf / 10.f, pierde = menosinf / 10.f;
const int num_pieces = 2;
const int PROFUNDIDAD_MINIMAX = 4; // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta
const int PROFUNDIDAD_MAXIMA = 10;// Umbral maximo de profundidad al que puede llegar de manera dinamica para la poda Alfa_Beta_ProfundidadDinamica

float Minimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int
&id_piece, int &dice, Heuristic *heuristic)
{
   if (profundidad == profundidad_max || actual.gameOver())
   {  // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   { // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      //PARCHIS-SIS
      // Obtener los hijos del nodo actual
      //vector<ParchisSis> rama = actual.getChildrenList();

      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            if(profundidad == 0)
            {
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }
            return valor;
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, heuristic);
         if (new_val > valor)
         {
            // Me voy quedando con el máximo
            valor = new_val;
            if (profundidad == 0)
            {
               // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }
         }
      }

      //PARCHIS-SIS
      // Cabecera del bucle con el vector de hijos
      // for(int i = 0; i < rama.size(); i++)
      // {
      //    ParchisSis hijo_i = rama[i]; // Acceso al estado con el hijo i-ésimo del vector
      //    Parchis nuevo_hijo = *hijo_i; // Acceso al estado
         
      //    // Verificar si hemos alcanzado el límite
      //    if (NodeCounter::isLimitReached())
      //    {
      //       cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
      //       if(profundidad == 0)
      //       {
      //          c_piece = hijo_i.getMovedColor();
      //          id_piece = hijo_i.getMovedPieceId();
      //          dice = hijo_i.getMovedDiceValue();     
      //       }
      //       return valor;
      //    }

      //    // Búsqueda en profundidad (llamada recursiva)
      //    float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, heuristic);

      //    if (new_val > valor)
      //    {
      //       // Me voy quedando con el máximo
      //       valor = new_val;
      //       if (profundidad == 0)
      //       {
      //          // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
      //          c_piece = hijo_i.getMovedColor();
      //          id_piece = hijo_i.getMovedPieceId();
      //          dice = hijo_i.getMovedDiceValue();
      //       }
      //    }
      // }

      return valor;
   }
   else
   { // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            return heuristic->evaluate(actual, jugador);
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
         heuristic);
         // Me voy quedando con el mínimo
         if (new_val < valor)
         {
            valor = new_val;
         }
      }
      return valor;
   }
}

float AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, float alpha, float beta, Heuristic *heuristic) const
{
   if (profundidad == profundidad_max || actual.gameOver())
   {  // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   { // Nodo MAX
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      for (ParchisBros::Iterator it = rama.begin(); it != rama.end() and alpha < beta; ++it)
      {
         Parchis nuevo_hijo = *it;

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            if(profundidad == 0)
            {
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }
            return alpha;
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Poda_AlfaBeta(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
         
         if(new_val > alpha){
            alpha = new_val;

            if (profundidad == 0)
            {
               // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }

            if(alpha >= beta){
               return beta;
            }
         }
      
      }

      return alpha;
   }
   else
   { // Nodo MIN
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end() and alpha < beta; ++it)
      {
         Parchis nuevo_hijo = *it;

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            return heuristic->evaluate(actual, jugador);
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Poda_AlfaBeta(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

         if(new_val < beta){
            beta = new_val;

            if(beta <= alpha){
               return alpha;
            }
         }
         
      }
      return beta;
   }
}

int estimarNuevaProfundidadMax(int profundidad, int profundidad_max, int ramificacion_max) {
   int nueva_profundidad_max = profundidad_max;

   if(profundidad <= 1){
      //Calcular nodos restantes
      constexpr int NODOS_MAXIMOS_ESTIMADOS = 1500000;
      int nodos_restantes = max(1, NODOS_MAXIMOS_ESTIMADOS - NodeCounter::getGenerated());

      //Estimar profundidad extra
      int profundidad_extra = 0;
      if (ramificacion_max > 0) {
         profundidad_extra = floor(log(nodos_restantes) / log(ramificacion_max + 1));
      }

      // Limitar cuánto se puede extender
      const int PROFUNDIDAD_EXTRA_MAX = 2; // evita extender demasiado
      profundidad_extra = min(profundidad_extra, PROFUNDIDAD_EXTRA_MAX);

      //Nueva profundidad
      if(profundidad_max < PROFUNDIDAD_MAXIMA){
         nueva_profundidad_max = profundidad_max + profundidad_extra;
      }
   } else if (ramificacion_max <= 3 and profundidad_max < PROFUNDIDAD_MAXIMA){
      nueva_profundidad_max = profundidad_max + 1;
   }

   return nueva_profundidad_max;
}

float AIPlayer::Poda_AlfaBeta_ProfundidadDinamica(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, float alpha, float beta, Heuristic *heuristic) const
{
   if (profundidad == profundidad_max || actual.gameOver())
   {  // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   { // Nodo MAX

      //PARCHIS-SIS
      // Obtener los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      //Numero de hijos
      int ramificacion_max=rama.size();
      int nueva_profundidad_max = profundidad_max;
      if (ramificacion_max <= 3 and profundidad_max < PROFUNDIDAD_MAXIMA){
         nueva_profundidad_max = profundidad_max + 1;
      }

      //else if(ramificacion_max > 15){
      //    nueva_profundidad_max = PROFUNDIDAD_ALFABETA;
      // }

      //cout << "Profundidad: " << profundidad << " HijosMAX: " << rama.size() << " nuevaProfundidad: " << nueva_profundidad_max << endl;
      
      // if (nueva_profundidad_max > PROFUNDIDAD_MAXIMA){
      //    nueva_profundidad_max = PROFUNDIDAD_MAXIMA;
      // }

      // if(nueva_profundidad_max > PROFUNDIDAD_ALFABETA){
      //    if(ramificacion_max > 27){
      //       nueva_profundidad_max -= 3;
      //    }else if(ramificacion_max > 22){
      //       nueva_profundidad_max -= 2;
      //    }else if(ramificacion_max > 15){
      //       nueva_profundidad_max -= 1;
      //    }
      // }

      //PARCHIS-SIS
      // Cabecera del bucle con el vector de hijos
      for(int i = 0; i < rama.size() and alpha < beta; i++)
      {
         ParchisSis hijo_i = rama[i]; // Acceso al estado con el hijo i-ésimo del vector
         Parchis nuevo_hijo = *hijo_i; // Acceso al estado

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            if(profundidad == 0)
            {
               c_piece = hijo_i.getMovedColor();
               id_piece = hijo_i.getMovedPieceId();
               dice = hijo_i.getMovedDiceValue(); 
            }
            return alpha;
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Poda_AlfaBeta_ProfundidadDinamica(nuevo_hijo, jugador, profundidad + 1, nueva_profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
         
         if(new_val > alpha){
            alpha = new_val;

            if (profundidad == 0)
            {
               // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
               c_piece = hijo_i.getMovedColor();
               id_piece = hijo_i.getMovedPieceId();
               dice = hijo_i.getMovedDiceValue();
            }

            if(alpha >= beta){
               return beta;
            }
         }
      
      }

      return alpha;
   }
   else
   { // Nodo MIN
      //PARCHIS-SIS
      // Obtener los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      int ramificacion_max=rama.size();
      int nueva_profundidad_max = profundidad_max;
      if (ramificacion_max <= 3 and profundidad_max < PROFUNDIDAD_MAXIMA){
         nueva_profundidad_max = profundidad_max + 1;
      }

      // else if(ramificacion_max > 15){
      //    nueva_profundidad_max = PROFUNDIDAD_ALFABETA;
      // }

      //cout << "Profundidad: " << profundidad << " HijosMAX: " << rama.size() << " nuevaProfundidad: " << nueva_profundidad_max << endl;

      // if (nueva_profundidad_max > PROFUNDIDAD_MAXIMA){
      //    nueva_profundidad_max = PROFUNDIDAD_MAXIMA;
      // }

      // if(nueva_profundidad_max > PROFUNDIDAD_ALFABETA){
      //    if(ramificacion_max > 27){
      //       nueva_profundidad_max -= 3;
      //    }else if(ramificacion_max > 22){
      //       nueva_profundidad_max -= 2;
      //    }else if(ramificacion_max > 15){
      //       nueva_profundidad_max -= 1;
      //    }
      // }

      //PARCHIS-SIS
      // Cabecera del bucle con el vector de hijos
      for(int i = 0; i < rama.size() and alpha < beta; i++)
      {
         ParchisSis hijo_i = rama[i]; // Acceso al estado con el hijo i-ésimo del vector
         Parchis nuevo_hijo = *hijo_i; // Acceso al estado

         // Verificar si hemos alcanzado el límite
         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
            return heuristic->evaluate(actual, jugador);
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Poda_AlfaBeta_ProfundidadDinamica(nuevo_hijo, jugador, profundidad + 1, nueva_profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

         if(new_val < beta){
            beta = new_val;

            if(beta <= alpha){
               return alpha;
            }
         }
         
      }
      return beta;
   }
}

float AIPlayer::Poda_AlfaBeta_OrdenacionMov(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, float alpha, float beta, Heuristic *heuristic) const
{
   if (profundidad == profundidad_max || actual.gameOver())
   {  // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   { // Nodo MAX
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      if(profundidad <= 1){
         //Antes de expandir hijos, evaluar cada uno con una heuristica
         //Nodo MAX: Ordenar de mayor a menor valor
         vector<pair<float, ParchisBros::Iterator>> hijos_valorados;
         for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it) {
            hijos_valorados.push_back({heuristic->evaluate(*it, jugador), it});
         }

         // Ordenar de mayor a menor 
         sort(hijos_valorados.begin(), hijos_valorados.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
         });

         for (const auto& par : hijos_valorados) {
            if (alpha >= beta) break;

            ParchisBros::Iterator it = par.second;
            Parchis nuevo_hijo = *(par.second);
            
            // Verificar si hemos alcanzado el límite
            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
               if(profundidad == 0)
               {
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }
               return alpha;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = Poda_AlfaBeta_OrdenacionMov(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
            
            if(new_val > alpha){
               alpha = new_val;

               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }

               if(alpha >= beta){
                  return beta;
               }
            }
         }
      } else{ //Si no estamos en la raiz(primer nivel de búsqueda)
         for (ParchisBros::Iterator it = rama.begin(); it != rama.end() and alpha < beta; ++it)
         {
            Parchis nuevo_hijo = *it;

            // Verificar si hemos alcanzado el límite
            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
               if(profundidad == 0)
               {
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }
               return alpha;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = Poda_AlfaBeta_OrdenacionMov(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
            
            if(new_val > alpha){
               alpha = new_val;

               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }

               if(alpha >= beta){
                  return beta;
               }
            }
         
         }
      }

      return alpha;
   }
   else
   { // Nodo MIN
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      if(profundidad <= 1){
         //Antes de expandir hijos, evaluar cada uno con una heuristica
         //Nodo MIN: Ordenar de menor a mayor valor
         vector<pair<float, ParchisBros::Iterator>> hijos_valorados;
         for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it) {
            hijos_valorados.push_back({heuristic->evaluate(*it, jugador), it});
         }

         // Ordenar de menor a mayor 
         sort(hijos_valorados.begin(), hijos_valorados.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
         });

         for (const auto& par : hijos_valorados) {
            if (alpha >= beta) break;

            Parchis nuevo_hijo = *(par.second);
   
            // Verificar si hemos alcanzado el límite
            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
               return heuristic->evaluate(actual, jugador);
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = Poda_AlfaBeta_OrdenacionMov(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

            if(new_val < beta){
               beta = new_val;

               if(beta <= alpha){
                  return alpha;
               }
            }
         }

      }else{ //Si no estamos en la raiz(primer nivel de búsqueda)
         for (ParchisBros::Iterator it = rama.begin(); it != rama.end() and alpha < beta; ++it)
         {
            Parchis nuevo_hijo = *it;
   
            // Verificar si hemos alcanzado el límite
            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;
               return heuristic->evaluate(actual, jugador);
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = Poda_AlfaBeta_OrdenacionMov(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

            if(new_val < beta){
               beta = new_val;

               if(beta <= alpha){
                  return alpha;
               }
            }
            
         }
      }
      return beta;
   }
}

bool AIPlayer::move(){
   cout << COUT_ORANGE_BOLD << "Realizo un movimiento automatico" << COUT_NOCOLOR << endl;

   color c_piece;
   int id_piece;
   int dice;
   think(c_piece, id_piece, dice);

   cout << COUT_ORANGE_BOLD << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << COUT_NOCOLOR << endl;

   actual->movePiece(c_piece, id_piece, dice);
   return true;
}

void AIPlayer::think(color& c_piece, int& id_piece, int& dice) const{
   // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
   //respectivamente, de:
   // - color de ficha a mover
   // - identificador de la ficha que se va a mover
   // - valor del dado con el que se va a mover la ficha.
   
   float valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
   float alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
   // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
   ValoracionTest valoracionTest;
   miValoracion1 valoracion1;
   miValoracion2 valoracion2;
   miValoracion3 valoracion3;
   miValoracion4 valoracion4;
   miValoracion5 valoracion5;

   switch(id){
   case 0:
      valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracionTest);
      break;
   case 1:
         // Mi implementación definitiva con la que gano a todos los ninjas.
      valor = Poda_AlfaBeta_OrdenacionMov(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion5);
      break;
   case 2:
      thinkMejorOpcion(c_piece, id_piece, dice);
      break;
   case 3:
      valor = Minimax(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion1);
      break;
   case 4:
      valor = Poda_AlfaBeta_ProfundidadDinamica(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion1);
   case 5:
      valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion1);
      break;
   case 6:
      valor = Minimax(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion4);
      break;
   case 7: //ganamos a ninja 2 con la poda como jugador 1 
      //gana a case 5: Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion1); tanto como jugador 1 como 2
      valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion4);
      break;
   case 8: //poda alfabeta gana al ninja 1 como jugador 1 y 2, con profundidad dinamica pierda como jugador 2 pierde ¿PORQUE?
      //ganamos a ninja 2 con la poda como jugador 1 y como jugador 2
      valor = Poda_AlfaBeta_ProfundidadDinamica(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion4);
      break;
   case 9: //gana como jugador 1 a ninja 3
   //hay veces q se pasa de maximo de nodos con PROFUNDIDAD_ALFABETA=7
   //con PROFUNDIDAD_ALFABETA=6 no se pasa y gana a todos menos a JUGADOR 2 VS NINJA 2, con PROFUNDIDAD_MAXIMA = 10
      valor = Poda_AlfaBeta_ProfundidadDinamica(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion5);  
      break;
   case 10:  //gana como jugador 2 a ninja 3, pierde como jugador 1 contra ninja 1 y 2. Si bajamos la PROFUNDIDAD_ALFABETA de 8 a 7 pierde un monton
      valor = Poda_AlfaBeta_OrdenacionMov(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion5);
      break;
   case 11: //con PROFUNDIDAD_ALFABETA = 8 gana 2 y con PROFUNDIDAD_ALFABETA = 7 gana a los mismos
      valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion5);
      break;    
   case 12:
      valor = Poda_AlfaBeta_OrdenacionMov(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion1);
      break;
   case 13:
      valor = Poda_AlfaBeta_OrdenacionMov(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracion4);
      break;
   }

 
   cout << "Valor MiniMax: " << valor << " Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

   // ----------------------------------------------------------------- //

   // Si quiero poder manejar varios comportamientos, puedo usar la variable id del agente para usar una u otra.
   // switch (id)
   // {
   // case 0:
   //    // Mi implementación base de la poda con ValoracionTest
   //    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracionTest);
   //    break;
   // case 1:
   //    // Mi implementación definitiva con la que gano a todos los ninjas.
   //    valor = Poda_Final2DefinitivaAhoraSi(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion3);
   //    break;
   // case 2:
   //    // Las distintas pruebas que he realizado (primera prueba)
   //    valor = Poda_AlfaBeta_Mejorada(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion1);
   //    break;
   // case 3:
   //    // Las distintas pruebas que he realizado (segunda prueba)
   //    valor = Poda_AlfaBeta_SegundaMejora(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion1);
   //    break;
   //  // ...
   // }

}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const{
   // IMPLEMENTACIÓN INICIAL DEL AGENTE
   // Esta implementación realiza un movimiento aleatorio.
   // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
   //como lo que se muestran al final de la función.
   
   // El id de mi jugador actual.
   int player = actual->getCurrentPlayerId();

   // Vector que almacenará los dados que se pueden usar para el movimiento
   vector<int> current_dices;
   // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
   vector<tuple<color, int>> current_pieces;

   // Se obtiene el vector de dados que se pueden usar para el movimiento
   current_dices = actual->getAvailableNormalDices(player);
   // Elijo un dado de forma aleatoria.
   dice = current_dices[rand() % current_dices.size()];

   // Se obtiene el vector de fichas que se pueden mover para el dado elegido
   current_pieces = actual->getAvailablePieces(player, dice);

   // Si tengo fichas para el dado elegido muevo una al azar.
   if (current_pieces.size() > 0){
      int random_id = rand() % current_pieces.size();
      id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
      c_piece = get<0>(current_pieces[random_id]); // elemento de la tupla
   }
   else{
      // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }
}
void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const{
   // Elijo el dado haciendo lo mismo que el jugador aleatorio.
   thinkAleatorio(c_piece, id_piece, dice);
 
   // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.
   // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
   // el dado especial a) la que esté más adelantada
   // (equivalentemente, la más cercana a la meta).
   int player = actual->getCurrentPlayerId();
   vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
   int id_ficha_mas_adelantada = -1;
   color col_ficha_mas_adelantada = none;
   int min_distancia_meta = 9999;

   for (int i = 0; i < current_pieces.size(); i++)
   {
      // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
      color col = get<0>(current_pieces[i]);
      int id = get<1>(current_pieces[i]);
      int distancia_meta = actual->distanceToGoal(col, id);
      if (distancia_meta < min_distancia_meta)
      {
      min_distancia_meta = distancia_meta;
      id_ficha_mas_adelantada = id;
      col_ficha_mas_adelantada = col;
      }
   }
   // Si no he encontrado ninguna ficha, paso turno.
   if (id_ficha_mas_adelantada == -1)
   {
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }
   // En caso contrario, moveré la ficha más adelantada.
   else
   {
      id_piece = id_ficha_mas_adelantada;
      c_piece = col_ficha_mas_adelantada;
   }
}
void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const{
   // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.
   // Cuando ya he recorrido todos los hijos, la función devuelve el estado actual. De esta forma puedo saber
   // cuándo paro de iterar.
   // Para ello, vamos a iterar sobre los hijos con la función de Parchis getChildren().
   // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
   // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.
   ParchisBros hijos = actual->getChildren();
   bool me_quedo_con_esta_accion = false;

   // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
   // Al hacer begin() accedemos al primer hijo de la rama,
   // y cada vez que hagamos ++it saltaremos al siguiente hijo.
   // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.
   for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() and !me_quedo_con_esta_accion; ++it)
   {
      Parchis siguiente_hijo = *it;
      // Si en el movimiento elegido comiera ficha, llegara a la meta o ganara, me quedo con esa acción.
      // Termino el bucle en este caso.
      if (siguiente_hijo.isEatingMove() or siguiente_hijo.isGoalMove() or (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador))
      {
         me_quedo_con_esta_accion = true;
         c_piece = it.getMovedColor();
         id_piece = it.getMovedPieceId();
         dice = it.getMovedDiceValue();
      }
   }
   // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
   // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
   // Si no he encontrado ninguna acción que me interese, voy a mover la ficha más adelantada como en el caso anterior.
   if(!me_quedo_con_esta_accion){
      thinkFichaMasAdelantada(c_piece, id_piece, dice);
   }
}


float ValoracionTest::getHeuristic(const Parchis& estado, int jugador) const{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;
      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++){
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)){
               puntuacion_jugador++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal){
               puntuacion_jugador += 5;
            }
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;
      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++){
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            if (estado.isSafePiece(c, j)){
               // Valoro negativamente que la ficha esté en casilla segura o meta.
               puntuacion_oponente++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal){
               puntuacion_oponente += 5;
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
}

float miValoracion1::getHeuristic(const Parchis& estado, int jugador) const{

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      int puntuacion_jugador=0;
      for (int i = 0; i < my_colors.size(); i++){
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            puntuacion_jugador += estado.distanceToGoal(c,j);
         }
      }

      int puntuacion_oponente=0;
      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++){
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            puntuacion_oponente += estado.distanceToGoal(c,j);
         }
      }

      // Devuelvo la puntuación de mi oponente menos la puntuación de mi jugador.
      return puntuacion_oponente - puntuacion_jugador;
   }
}

float miValoracion2::getHeuristic(const Parchis& estado, int jugador) const{
   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      int puntuacion_total = 0;

      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // ----- DISTANCIA A META -----
      int puntuacion_jugador=0;
      for (int i = 0; i < my_colors.size(); i++){
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            puntuacion_jugador += estado.distanceToGoal(c,j);
         }
      }

      int puntuacion_oponente=0;
      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++){
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            puntuacion_oponente += estado.distanceToGoal(c,j);
         }
      }

      puntuacion_total += puntuacion_oponente - puntuacion_jugador;

      // ----- FICHAS EN META -----
      int mis_metas = 0, metas_oponente = 0;
      for (int i = 0; i < my_colors.size(); i++){
         mis_metas += estado.piecesAtGoal(my_colors[i]);
      }

      for (int i = 0; i < op_colors.size(); i++){
         metas_oponente += estado.piecesAtGoal(op_colors[i]);
      }

      puntuacion_total += (mis_metas - metas_oponente)*100;

      // ----- FICHAS EN CASA -----
      int mis_casa = 0, casa_oponente = 0;
      for (int i = 0; i < my_colors.size(); i++){
         mis_casa += estado.piecesAtHome(my_colors[i]);
      }

      for (int i = 0; i < op_colors.size(); i++){
         casa_oponente += estado.piecesAtHome(op_colors[i]);
      }

      puntuacion_total += (casa_oponente - mis_casa)*10;

      // ----- FICHAS EN CASILLA SEGURA -----
      int seguras_mias = 0, seguras_oponente = 0;
      for (int i = 0; i < my_colors.size(); i++){
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            if (estado.isSafePiece(c, j)){
               seguras_mias++;
            }
         }
      }
      
      for (int i = 0; i < op_colors.size(); i++){
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            if (estado.isSafePiece(c, j)){
               seguras_oponente++;
            }
         }
      }

      puntuacion_total += (seguras_mias - seguras_oponente)*5;

      // ----- FICHAS COMIDAS ESTE TURNO -----
      tuple<color, int, int> last_action = estado.getLastAction();

      switch (get<0>(last_action))
      {
      case red:
      case blue:
         if(estado.eatenPiece().first == yellow or estado.eatenPiece().first == green){
            puntuacion_total += 50; // He comido
         } else if(estado.eatenPiece().first != none){
               puntuacion_total -= 45; //me como a mi mismo
         }
         break;
      case green:
      case yellow:
         if(estado.eatenPiece().first == red or estado.eatenPiece().first == blue){
            puntuacion_total += 50; // He comido
         } else if(estado.eatenPiece().first != none){
               puntuacion_total -= 45; //me como a mi mismo
         }
         break;
      case none:
         break;
      }

      return puntuacion_total;
   }
}

float miValoracion3::getHeuristic(const Parchis& estado, int jugador) const {
   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{

      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      int dist_jugador = 0, dist_oponente = 0;
      int metas_jugador = 0, metas_oponente = 0;

      for (color c : my_colors) {
         metas_jugador += estado.piecesAtGoal(c);
         for (int j = 0; j < num_pieces; j++) {
               dist_jugador += estado.distanceToGoal(c, j);
         }
      }

      for (color c : op_colors) {
         metas_oponente += estado.piecesAtGoal(c);
         for (int j = 0; j < num_pieces; j++) {
               dist_oponente += estado.distanceToGoal(c, j);
         }
      }

      return (dist_oponente - dist_jugador) + 20*(metas_jugador - metas_oponente);
   }
}

float miValoracion4::getHeuristic(const Parchis& estado, int jugador) const{
   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      int distancia_mia = 0, distancia_oponente = 0;
      int metas_mias = 0, metas_oponente = 0;
      int casa_mia = 0, casa_oponente = 0;
      int seguras_mias = 0, seguras_oponente = 0;

      // Recorrer fichas del jugador y del oponente
      for (int i = 0; i < my_colors.size(); ++i) {
         color c = my_colors[i];
         for (int j = 0; j < num_pieces; ++j) {
               distancia_mia += estado.distanceToGoal(c, j);
               if (estado.isSafePiece(c, j))
                  seguras_mias++;
         }
         metas_mias += estado.piecesAtGoal(c);
         casa_mia += estado.piecesAtHome(c);
      }

      for (int i = 0; i < op_colors.size(); ++i) {
         color c = op_colors[i];
         for (int j = 0; j < num_pieces; ++j) {
               distancia_oponente += estado.distanceToGoal(c, j);
               if (estado.isSafePiece(c, j))
                  seguras_oponente++;
         }
         metas_oponente += estado.piecesAtGoal(c);
         casa_oponente += estado.piecesAtHome(c);
      }

      // --------- EVALUACIÓN ---------

      float score = 0;

      // 1. Distancia total a meta
      score += (distancia_oponente - distancia_mia); // mayor si yo estoy más cerca

      // 2. Fichas en meta
      score += (metas_mias - metas_oponente) * 20;

      // 3. Penalización por fichas en casa
      score += (casa_oponente - casa_mia) * 5;

      // 4. Casillas seguras
      score += (seguras_mias - seguras_oponente) * 2;

      // 5. Comer o ser comido
      auto comida = estado.eatenPiece();
      if (comida.first != none) {
         bool yo_comi = find(my_colors.begin(), my_colors.end(), comida.first) == my_colors.end();
         bool me_comieron = find(my_colors.begin(), my_colors.end(), comida.first) != my_colors.end();

         if (yo_comi) score += 15;
         if (me_comieron) score -= 12;
      }

      return score;

   }
}

float miValoracion5::getHeuristic(const Parchis& estado, int jugador) const{
   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      float score = 0;

      int distancia_mia = 0, distancia_oponente = 0;
      int metas_mias = 0, metas_oponente = 0;
      int casa_mia = 0, casa_oponente = 0;
      int seguras_mias = 0, seguras_oponente = 0;

      // Recorrer fichas del jugador y del oponente
      for (int i = 0; i < my_colors.size(); ++i) {
         color c = my_colors[i];
         for (int j = 0; j < num_pieces; ++j) {
               distancia_mia += estado.distanceToGoal(c, j);
               if (estado.isSafePiece(c, j))
                  seguras_mias++;

               //Fichas rivales en el camino
               const Box& desde = estado.getBoard().getPiece(c, j).get_box();
               const Box& meta = Box(1, final_queue, c);
               auto piezas = estado.allPiecesBetween(desde, meta);
               for (auto& p : piezas) {
                  if (find(op_colors.begin(), op_colors.end(), p.first) != op_colors.end()) {
                     score -= 2; // penaliza fichas rivales en el camino
                  }
               }

               //Barreras
               vector<color> barreras = estado.anyWall(desde, meta);
               if(barreras.size()>0){
                  for (color b : barreras) {
                     if (find(op_colors.begin(), op_colors.end(), b) != op_colors.end()) {
                        score -= 15; // Barrera enemiga en el camin
                     }else{
                        score += 8; // Barrera propia
                     }
                  }  
               }
               
         }
         metas_mias += estado.piecesAtGoal(c);
         casa_mia += estado.piecesAtHome(c);
      }

      for (int i = 0; i < op_colors.size(); ++i) {
         color c = op_colors[i];
         for (int j = 0; j < num_pieces; ++j) {
               distancia_oponente += estado.distanceToGoal(c, j);
               if (estado.isSafePiece(c, j))
                  seguras_oponente++;
         }
         metas_oponente += estado.piecesAtGoal(c);
         casa_oponente += estado.piecesAtHome(c);
      }

      // --------- EVALUACIÓN ---------

      // 1. Distancia total a meta
      score += (distancia_oponente - distancia_mia); // mayor si yo estoy más cerca

      // 2. Fichas en meta
      score += (metas_mias - metas_oponente) * 20;

      // 3. Penalización por fichas en casa
      score += (casa_oponente - casa_mia) * 5;

      // 4. Casillas seguras
      score += (seguras_mias - seguras_oponente) * 2;

      // 5. Comer o ser comido
      auto comida = estado.eatenPiece();
      if (comida.first != none) {
         bool yo_comi = find(my_colors.begin(), my_colors.end(), comida.first) == my_colors.end();
         bool me_comieron = find(my_colors.begin(), my_colors.end(), comida.first) != my_colors.end();

         if (yo_comi) score += 30;
         if (me_comieron) score -= 24;
      }

      //FINALES
      bool en_finales = (metas_mias >= 2) || (metas_mias + casa_mia >= 3 && distancia_mia < distancia_oponente);
      if (en_finales) {
         score += metas_mias * 10;
         score -= casa_mia * 7;
         score -= distancia_mia * 0.3;
         score += seguras_mias * 2;
      }

      bool rival_en_finales = metas_oponente >= 2;
      if (rival_en_finales) {
         score -= metas_oponente * 10;
         score += casa_oponente * 3;
      }

      return score;

   }
}