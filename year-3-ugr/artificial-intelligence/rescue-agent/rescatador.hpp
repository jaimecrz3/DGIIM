#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <algorithm> 

#include "comportamientos/comportamiento.hpp"
using namespace std;
#include <iostream>

struct EstadoR{
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoR &st) const{
    return f == st.f && c == st.c && brujula == st.brujula and zapatillas == st.zapatillas;
  }
  bool operator<(const EstadoR &st) const{
    if (f < st.f) return true;
    else if (f == st.f and c < st.c) return true;
    else if (f == st.f and c == st.c and brujula < st.brujula) return true;
    else if (f == st.f and c == st.c and brujula == st.brujula and zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoR{
  EstadoR estado;
  list<Action> secuencia;
  int coste = 0;

  bool operator==(const NodoR &node) const{
    return estado == node.estado;
  }
  // bool operator<(const NodoR &node) const{
  //   if (estado.f < node.estado.f) return true;
  //   else if (estado.f == node.estado.f and estado.c < node.estado.c) return true;
  //   else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula < node.estado.brujula) return true;
  //   else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula == node.estado.brujula and estado.zapatillas < node.estado.zapatillas) return true;
  //   else return false;
  // }
  bool operator<(const NodoR &node) const {
    return coste > node.coste;  // INVERTIDO para que la cola de prioridad extraiga el menor coste
  }
};

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45Izq = 0;
    giro180=0;
    andar=0;
    giro45dcha=0;
    for(int i=0; i<4; i++){
      cuadrantes.push_back(0);
    }
    
    lado_cuadrante = size/2;
    giroAlterno = 0;
    tamanio_mapa=size;
    instante_simulacion=3000;
    instantes_simulacion_auxiliar=0;
    sensores_validos=false;
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan=false;
    last_action = IDLE;
    tiene_zapatillas = false;
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);

  char ViablePorAlturaR(char casilla, int dif, bool zap);
  int VeoCasillaIntersanteR(char i, char c, char d, bool zap, char a, char a_i, char a_d,char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual);
  int VeoCasillaIntersanteR1(char i, char c, char d, bool zap, char a, char a_i, char a_d,char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual, bool run);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);

  bool CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Action accion);
  EstadoR applyR(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoR &st, const list<Action> &plan);
  void PintaPlan(const list<Action> &plan, bool zap);
  int fixBateria_sig_accion_jugador(unsigned char celdaJugador, int difAltura, Action accion);
  list<Action> Dijkstra(const EstadoR &inicio, const EstadoR &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);


  Action ComportamientoRescatadorNivel_3(Sensores sensores);

  bool CasillaAccesibleRescatadorN4(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Action accion, Sensores sensores, bool sensores_validos);
  EstadoR applyRN4(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Sensores sensores, bool sensores_validos);
  list<Action> DijkstraN4(const EstadoR &inicio, const EstadoR &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura, Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);

private:
  // Variables de Estado
  //Inicializar variables
  Action last_action; //Almacena la ultima accion realizada por el agente
  bool tiene_zapatillas; //Indica si ya paso por una casilla que tiene zapatillas
  int giro45Izq; //Indica si esta haciendo TURN_SL(0 indica que no)
  int giro180;
  int andar;
  //Almacena las vece consecutivas que esta en cada cuadrante, 
  // cuadrantes[0] -> esquina superior izq
  // cuadrantes[1] -> esquina superior dcha
  // cuadrantes[2] -> esquina inf dcha
  // cuadrantes[3] -> esquina inf izq
  vector<int> cuadrantes; 
  int lado_cuadrante;
  int giro45dcha;
  map<pair<int, int>, Action> ultimaAccionPorCasilla;
  map<pair<int, int>, Orientacion> ultimaOrientacionPorCasilla;
  map<pair<int, int>, int> visitasPorCasilla;
  map<pair<int, int>, int> visitasPorCasillaBucle;
  map<pair<int, int>, int> buclePorCasilla;
  deque<pair<int,int>> historial;
  int giroAlterno;

   //variables nivel 2
  list<Action> plan;
  bool hayPlan;

  unsigned int tamanio_mapa;
  int instante_simulacion;
  int instantes_simulacion_auxiliar;
  bool sensores_validos;

  
};

#endif
