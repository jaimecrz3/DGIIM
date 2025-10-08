#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <map>
#include <thread>
#include <list>
#include <queue>
#include <set>
#include <algorithm> 
#include <map>

#include "comportamientos/comportamiento.hpp"

using namespace std;
#include <iostream>

struct EstadoAuxiliar {
    int turnosQuietoRescatador = 0;
    int ultimaPosRescatador = -1;
};

struct EstadoA{
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoA &st) const{
    return f == st.f && c == st.c && brujula == st.brujula and zapatillas == st.zapatillas;
  }
};

struct NodoA{
  EstadoA estado;
  list<Action> secuencia;
  bool operator==(const NodoA &node) const{
    return estado == node.estado;
  }
  bool operator<(const NodoA &node) const{
    if (estado.f < node.estado.f) return true;
    else if (estado.f == node.estado.f and estado.c < node.estado.c) return true;
    else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula < node.estado.brujula) return true;
    else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula == node.estado.brujula and estado.zapatillas < node.estado.zapatillas) return true;
    else return false;
  }
};

struct EstadoAEstrella{
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoAEstrella &st) const{
    return f == st.f && c == st.c && brujula == st.brujula and zapatillas == st.zapatillas;
  }
  bool operator<(const EstadoAEstrella &st) const{
    if (f < st.f) return true;
    else if (f == st.f and c < st.c) return true;
    else if (f == st.f and c == st.c and brujula < st.brujula) return true;
    else if (f == st.f and c == st.c and brujula == st.brujula and zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoAEstrella{
  EstadoAEstrella estado;
  vector<Action> secuencia;
  int coste=0;
  int estimacion = 0;

  bool operator==(const NodoAEstrella &node) const{
    return estado == node.estado;
  }
  // bool operator<(const NodoA &node) const{
  //   if (estado.f < node.estado.f) return true;
  //   else if (estado.f == node.estado.f and estado.c < node.estado.c) return true;
  //   else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula < node.estado.brujula) return true;
  //   else if (estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula == node.estado.brujula and estado.zapatillas < node.estado.zapatillas) return true;
  //   else return false;
  // }
  bool operator<(const NodoAEstrella &node) const {
        return estimacion > node.estimacion; // para que la menor estimación tenga mayor prioridad
    }
};

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45Izq = 0;
    andar=0;
    giro45dcha=0;
    for(int i=0; i<4; i++){
      cuadrantes.push_back(0);
    }
    
    lado_cuadrante = size/2;
    int giroAlterno=0;
    tamanio_mapa=size;
    instante_simulacion=3000;
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan=false;
    last_action = IDLE;
    tiene_zapatillas = false;
    size_t planIndex = 0;
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  void SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);

  char ViablePorAlturaA(char casilla, int dif);
  int VeoCasillaIntersanteA(char i, char c, char d, char r, char r_i, char r_d, char c5,char c6,char c7,  Orientacion orientacion, pair<int,int> posActual);
  int VeoCasillaIntersanteA1(char i, char c, char d, char r, char r_i, char r_d, char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual );

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);

  bool CasillaAccesibleAuxiliar(const EstadoAEstrella &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);
  EstadoAEstrella applyA(Action accion, const EstadoAEstrella & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);
  int fixBateria_sig_accion_jugador(unsigned char celdaJugador, int difAltura, Action accion);
  vector<Action> AEstrella(const EstadoAEstrella &inicio, const EstadoAEstrella &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);


  Action ComportamientoAuxiliarNivel_3(Sensores sensores);

  bool CasillaAccesibleAuxiliarN4(const EstadoAEstrella &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);
  EstadoAEstrella applyAN4(Action accion, const EstadoAEstrella & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);
  vector<Action> AEstrellaN4(const EstadoAEstrella &inicio, const EstadoAEstrella &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);

  Action ComportamientoAuxiliarNivel_E(Sensores sensores);
  list<Action> AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  list<Action> AnchuraAuxiliar_V2(const EstadoA &inicio, const EstadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoAEstrella &st, const vector<Action> &plan);
  void PintaPlan(const vector<Action> &plan, bool zap);

private:
  Action last_action; //Almacena la ultima accion realizada por el agente
  bool tiene_zapatillas; //Indica si ya paso por una casilla que tiene zapatillas
  int giro45Izq; //Indica si esta haciendo TURN_SL(0 indica que no)
  int andar;
  //Almacena las vece consecutivas que esta en cada cuadrante, 
  // cuadrantes[0] -> esquina superior izq
  // cuadrantes[1] -> esquina superior dcha
  // cuadrantes[2] -> esquina inf dcha
  // cuadrantes[3] -> esquina inf izq
  vector<int> cuadrantes; 
  int lado_cuadrante;
  int giro45dcha;
  int giroAlterno;
  map<pair<int, int>, int> ultimaAccionPorCasilla;
  map<pair<int, int>, Orientacion> ultimaOrientacionPorCasilla;
  map<pair<int, int>, int> visitasPorCasilla;
  map<pair<int, int>, int> buclePorCasilla;
  map<pair<int, int>, int> visitasPorCasillaBucle;

  //variables nivel E
  vector<Action> plan;
  bool hayPlan;
  size_t planIndex;

  int tamanio_mapa;
  int instante_simulacion;

};

#endif
