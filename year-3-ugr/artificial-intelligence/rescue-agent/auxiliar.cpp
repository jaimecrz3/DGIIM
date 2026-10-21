#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"



//2 fases:
//1) Una primera fase de obervacion donde se actualizan los cambios de la ultima accion
//2) Una segunda fase donde se decide la siguiente accion a realizar
Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		//accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3 (sensores);
		//accion = ComportamientoAuxiliarNivel_E (sensores);
		break;
	case 4:
		accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}



char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif){
	if(abs(dif)<= 1){
		return casilla;
	}else{
		return 'P';
	}
}

pair<int, int> posDelante(Orientacion o, pair<int,int> posActual){
    switch (o) {
        case norte:    return {posActual.first - 1, posActual.second};
        case noreste:  return {posActual.first - 1, posActual.second + 1};
        case este:     return {posActual.first, posActual.second + 1};
        case sureste:  return {posActual.first + 1, posActual.second + 1};
        case sur:      return {posActual.first + 1, posActual.second};
        case suroeste: return {posActual.first + 1, posActual.second - 1};
        case oeste:    return {posActual.first, posActual.second - 1};
        case noroeste: return {posActual.first - 1, posActual.second - 1};
    } 
}
pair<int, int> posIzq(Orientacion o, pair<int,int> posActual){
    switch (o) {
        case norte:    return {posActual.first - 1, posActual.second-1}; break;
		case noreste:  return {posActual.first - 1, posActual.second }; break;
		case este:    return {posActual.first-1, posActual.second + 1}; break;
		case sureste:  return {posActual.first, posActual.second + 1}; break;
		case sur:      return {posActual.first + 1, posActual.second+1}; break;
		case suroeste: return {posActual.first + 1, posActual.second }; break;
		case oeste:    return {posActual.first+1, posActual.second - 1}; break;
		case noroeste: return {posActual.first, posActual.second - 1}; break;
}
}
pair<int, int> posDcha(Orientacion o,  pair<int,int> posActual){
    switch (o) {
        case norte:    return  {posActual.first - 1, posActual.second+1}; break;
		case noreste:  return  {posActual.first, posActual.second+1 }; break;
		case este:     return {posActual.first+1, posActual.second + 1}; break;
		case sureste:  return {posActual.first+1, posActual.second}; break;
		case sur:      return {posActual.first + 1, posActual.second-1}; break;
		case suroeste: return {posActual.first , posActual.second -1}; break;
		case oeste:    return{posActual.first-1, posActual.second - 1}; break;
		case noroeste: return{posActual.first-1, posActual.second }; break;
    }
}

int ComportamientoAuxiliar::VeoCasillaIntersanteA(char i, char c, char d, char r, char r_i, char r_d, char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual ){

	if ( c == 'X' && visitasPorCasilla[posDelante(orientacion, posActual)] == 0 && r != 'r') return 2;
    if ( i == 'X' && visitasPorCasilla[posIzq(orientacion, posActual)] == 0 && r_i != 'r') return 1;
    if (d == 'X' && visitasPorCasilla[posDcha(orientacion, posActual)] == 0 && r_d != 'r') return 3;
	if (c == 'C'  && visitasPorCasilla[posDelante(orientacion, posActual)] == 0 && r != 'r') return 2;
    if (i == 'C'  && visitasPorCasilla[posIzq(orientacion, posActual)] == 0 && r_i != 'r') return 1;
    if (d == 'C'  && visitasPorCasilla[posDcha(orientacion, posActual)] == 0 && r_d != 'r') return 3;
	
	if(c=='X' && r!='r') return 2;
	else if(i=='X'&& r_i!='r') return 1;
	else if(d=='X'&& r_d!='r') return 3;
	else if(c=='C'&& r!='r'){
		if(orientacion==sureste||orientacion==noreste||orientacion==suroeste||orientacion==noroeste){
			return 2;
		}else if(c6=='C'||c6=='X'){
			return 2;
		}else if(i=='C'){
			return 5;
		}else if(d=='C'){
			return 6;
		}else if(i=='P'&&(c5=='C'||c5=='X')){
			return 2;
		}else if(d=='P'&&(c7=='C'||c7=='X')){
			return 2;
		}
	}
	else if(i=='C') return 1;
	else if(d=='C') return 3;
	//else if(c=='C'&& r=='r') return 4;
	else return 0;

}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	//1) Una primera fase de obervacion donde se actualizan los cambios de la ultima accion
	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);
	if(sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){ //noroeste
		cuadrantes[0]++;
	}else if(sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){ //noreste
		cuadrantes[1]++;
	}else if(sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){ //sureste
		cuadrantes[2]++;
	}else if(sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){ //suroeste
		cuadrantes[3]++;
	}
	// if(sensores.superficie[0] == 'D'){
	// 	tiene_zapatillas = true;
	// }
	bool rescatador_en_base = false;
	pair<int, int> posActual = {sensores.posF, sensores.posC};
	//Para el case 0
    pair<int, int> frente = {0, 0}; 
	int MAX_GIROS_ALTERNOS = 2;
	//Guardamos la acción, orientación y visitas
	ultimaAccionPorCasilla[posActual] = last_action;
	ultimaOrientacionPorCasilla[posActual] = sensores.rumbo;
	visitasPorCasilla[posActual]++;

	//2) Una segunda fase donde se decide la siguiente accion a realizar
	if(sensores.superficie[0] == 'X'){ //Llegue al objetivo
		accion = IDLE;
	}else if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	}else if(giro45dcha!=0){
		accion=TURN_SR;
		giro45dcha--;
	}else if(andar!=0){
		if(sensores.agentes[2]!='r'){
			accion = WALK;
		}else{
			accion = IDLE;
		}
		andar--;
	}else {
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);

		int pos = VeoCasillaIntersanteA(i,c,d, sensores.agentes[2], sensores.agentes[1],sensores.agentes[3],sensores.superficie[5], sensores.superficie[6], sensores.superficie[7], sensores.rumbo, posActual);
		frente = posDelante(sensores.rumbo, posActual);
		char terreno = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		bool casilla_frente_viable = ((terreno == 'C'||terreno == 'X') && visitasPorCasilla[frente] < 3 && sensores.agentes[2] != 'r');

		if (!casilla_frente_viable &&
			visitasPorCasilla[posActual] >10 &&
			ultimaOrientacionPorCasilla[posActual] == sensores.rumbo &&
			ultimaAccionPorCasilla[posActual] == WALK &&
			last_action == WALK)
		{
			buclePorCasilla[posActual]++;
			
			if (buclePorCasilla[posActual] >= 2) {
				if (giroAlterno < MAX_GIROS_ALTERNOS) {
					accion = TURN_SR;
					giroAlterno++;
				} else {
					pair<int, int> izq = posIzq(sensores.rumbo, posActual);
					char terreno_i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
					bool casilla_izq_viable = ((terreno_i == 'C'||terreno_i == 'X') && visitasPorCasilla[izq] < 3 && sensores.agentes[1] != 'r');

					pair<int, int> dcha= posDcha(sensores.rumbo, posActual);
					char terreno_d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
					bool casilla_dcha_viable = ((terreno_d == 'C'||terreno_d == 'X') && visitasPorCasilla[dcha] < 3 && sensores.agentes[3] != 'r');
					if(casilla_izq_viable&&casilla_dcha_viable){
						if(visitasPorCasilla[dcha]<=visitasPorCasilla[izq]){
							accion = TURN_SR;
							giroAlterno = 0; 
						}else{
							accion = TURN_SR;
							giro45Izq = 6;
							giroAlterno = 0; 
						}
					}else if(casilla_izq_viable){
						accion = TURN_SR;
						giro45Izq = 6;
						giroAlterno = 0; 
					}else if(casilla_dcha_viable){
						accion = TURN_SR;
						giroAlterno = 0; 
					}else{
						accion = TURN_SR;
						giro45Izq = 3;
						giroAlterno = 0;  
					}
				}
				visitasPorCasilla[posActual] -= 3;
				buclePorCasilla[posActual] = 0;
				last_action = accion;
				return accion;
			} else {
				accion = TURN_SR;
				giro45Izq = 5;
				last_action = accion;
				return accion;
			}
		}
		
		switch (pos)
			{
			case 2:
				for (int i = 1; i <= 8; i++) {
					if (sensores.agentes[i] == 'r') {
						if (sensores.superficie[i] == 'X') {
							rescatador_en_base = true;
							break;
						}
					}
				}
				switch (sensores.rumbo){
					case norte:
					case este:
					case sur:
					case oeste:
						accion = WALK;
						if((sensores.agentes[1]=='r'||sensores.agentes[3]=='r'||sensores.agentes[5]=='r'||sensores.agentes[6]=='r'||sensores.agentes[7]=='r')&&!rescatador_en_base){
							// if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[0]=0;
							// }else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[1]=0;
							// }else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[2]=0;
							// }else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[3]=0;
							// }else{
								accion = TURN_SR;
								giro45Izq=2;
							//}
						}
						break;
					case noreste:
					case noroeste:
					case sureste:
					case suroeste:
						accion = WALK;
						if((sensores.agentes[1]=='r'||sensores.agentes[3]=='r'||sensores.agentes[4]=='r'||sensores.agentes[5]=='r'||sensores.agentes[6]=='r'||sensores.agentes[7]=='r'||sensores.agentes[8]=='r')&&!rescatador_en_base){
							// if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[0]=0;
							// }else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[1]=0;
							// }else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[2]=0;
							// }else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[3]=0;
							// }else{
								accion = TURN_SR;
								giro45Izq=2;
							//}
						}
						break;
					
				}
				break;
			case 1:
				giro45Izq = 6;
				accion = TURN_SR;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 4:
				accion = IDLE;
				break;
			case 0: 
				if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[0]=0;
				}else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[1]=0;
				}else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[2]=0;
				}else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[3]=0;
				}else{
					giro45Izq = 3; //haceos giro 180º
					accion = TURN_SR;
				}
		
				break;
			case 5:
				giro45Izq = 6;
				accion = TURN_SR;
				andar=1;
				break;
			case 6:
				accion = TURN_SR;
				andar=1;
				break;
			}
		}

	last_action = accion;
	return accion;
}

int ComportamientoAuxiliar::VeoCasillaIntersanteA1(char i, char c, char d, char r, char r_i, char r_d, char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual ){

	if ( (c=='C'||c=='S') && visitasPorCasilla[posDelante(orientacion, posActual)] == 0 && r != 'r') return 2;
    if ((i=='C'||i=='S') && visitasPorCasilla[posIzq(orientacion, posActual)] == 0 && r_i != 'r') return 1;
    if ((d=='C'||d=='S')&& visitasPorCasilla[posDcha(orientacion, posActual)] == 0 && r_d != 'r') return 3;
	
	if((c=='C'||c=='S')&& r!='r'){
		if(orientacion==sureste||orientacion==noreste||orientacion==suroeste||orientacion==noroeste){
			return 2;
		}else if(c6=='C'||c6=='S'){
			return 2;
		}else if(i=='C'||i=='S'){
			return 5;
		}else if(d=='C'||d=='S'){
			return 6;
		}else if(i=='P'&&(c5=='C'||c5=='S')){
			return 2;
		}else if(d=='P'&&(c7=='C'||c7=='S')){
			return 2;
		}
	}
	else if(i=='C'||i=='S') return 1;
	else if(d=='C'||d=='S') return 3;
	//else if(c=='C'&& r=='r') return 4;
	else return 0;

}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{

	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	//1) Una primera fase de obervacion donde se actualizan los cambios de la ultima accion
	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);
	if(sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){ //noroeste
		cuadrantes[0]++;
	}else if(sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){ //noreste
		cuadrantes[1]++;
	}else if(sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){ //sureste
		cuadrantes[2]++;
	}else if(sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){ //suroeste
		cuadrantes[3]++;
	}
	// if(sensores.superficie[0] == 'D'){
	// 	tiene_zapatillas = true;
	// }
	pair<int, int> posActual = {sensores.posF, sensores.posC};
	//Para el case 0
    pair<int, int> frente = {0, 0}; 
	int MAX_GIROS_ALTERNOS = 2;
	//Guardamos la acción, orientación y visitas
	ultimaAccionPorCasilla[posActual] = last_action;
	ultimaOrientacionPorCasilla[posActual] = sensores.rumbo;
	visitasPorCasilla[posActual]++;

	//2) Una segunda fase donde se decide la siguiente accion a realizar
	if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	}else if(giro45dcha!=0){
		accion=TURN_SR;
		giro45dcha--;
	}else if(andar!=0){
		if(sensores.agentes[2]!='r'){
			accion = WALK;
		}else{
			accion = IDLE;
		}
		andar--;
	}else {
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);

		int pos = VeoCasillaIntersanteA1(i,c,d, sensores.agentes[2], sensores.agentes[1],sensores.agentes[3],sensores.superficie[5], sensores.superficie[6], sensores.superficie[7], sensores.rumbo, posActual);
		frente = posDelante(sensores.rumbo, posActual);
		char terreno = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		bool casilla_frente_viable = ((terreno == 'C'||terreno == 'X'||terreno=='S') && visitasPorCasilla[frente] < 3 && sensores.agentes[2] != 'r');

		if (!casilla_frente_viable &&
			visitasPorCasilla[posActual] >4 &&
			ultimaOrientacionPorCasilla[posActual] == sensores.rumbo &&
			ultimaAccionPorCasilla[posActual] == WALK &&
			last_action == WALK)
		{
			buclePorCasilla[posActual]++;
			
			if (buclePorCasilla[posActual] >= 2) {
				if (giroAlterno < MAX_GIROS_ALTERNOS) {
					accion = TURN_SR;
					giroAlterno++;
				} else {
					pair<int, int> izq = posIzq(sensores.rumbo, posActual);
					char terreno_i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
					bool casilla_izq_viable = ((terreno_i == 'C'||terreno_i == 'X'||terreno_i=='S') && visitasPorCasilla[izq] < 3 && sensores.agentes[1] != 'r');

					pair<int, int> dcha= posDcha(sensores.rumbo, posActual);
					char terreno_d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
					bool casilla_dcha_viable = ((terreno_d == 'C'||terreno_d == 'X'||terreno_d=='S') && visitasPorCasilla[dcha] < 3 && sensores.agentes[3] != 'r');
					if(casilla_izq_viable&&casilla_dcha_viable){
						if(visitasPorCasilla[dcha]<=visitasPorCasilla[izq]){
							accion = TURN_SR;
							giroAlterno = 0; 
						}else{
							accion = TURN_SR;
							giro45Izq = 6;
							giroAlterno = 0; 
						}
					}else if(casilla_izq_viable){
						accion = TURN_SR;
						giro45Izq = 6;
						giroAlterno = 0; 
					}else if(casilla_dcha_viable){
						accion = TURN_SR;
						giroAlterno = 0; 
					}else{
						accion = TURN_SR;
						giro45Izq = 3;
						giroAlterno = 0;  
					}
				}
				visitasPorCasilla[posActual] -= 3;
				buclePorCasilla[posActual] = 0;
				last_action = accion;
				return accion;
			} else {
				accion = TURN_SR;
				giro45Izq = 5;
				last_action = accion;
				return accion;
			}
		}
		
		switch (pos)
			{
			case 2:
				switch (sensores.rumbo){
					case norte:
					case este:
					case sur:
					case oeste:
						accion = WALK;
						if(sensores.agentes[1]=='r'||sensores.agentes[3]=='r'||sensores.agentes[5]=='r'||sensores.agentes[6]=='r'||sensores.agentes[7]=='r'){
							// if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[0]=0;
							// }else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[1]=0;
							// }else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[2]=0;
							// }else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[3]=0;
							// }else{
								accion = TURN_SR;
								giro45Izq=2;
							//}
						}
						break;
					case noreste:
					case noroeste:
					case sureste:
					case suroeste:
						accion = WALK;
						if(sensores.agentes[1]=='r'||sensores.agentes[3]=='r'||sensores.agentes[4]=='r'||sensores.agentes[5]=='r'||sensores.agentes[6]=='r'||sensores.agentes[7]=='r'||sensores.agentes[8]=='r'){
							// if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[0]=0;
							// }else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[1]=0;
							// }else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[2]=0;
							// }else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
							// 	giro45dcha=1;
							// 	accion = TURN_SR;
							// 	cuadrantes[3]=0;
							// }else{
								accion = TURN_SR;
								giro45Izq=2;
							//}
						}
						break;
					
				}
				break;
			case 1:
				giro45Izq = 6;
				accion = TURN_SR;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 4:
				accion = IDLE;
				break;
			case 0: 
				if(cuadrantes[0]>=20&&sensores.posF<lado_cuadrante&&sensores.posC<lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[0]=0;
				}else if(cuadrantes[1]>=20&&sensores.posF<lado_cuadrante&&sensores.posC>=lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[1]=0;
				}else if(cuadrantes[2]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC>=lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[2]=0;
				}else if(cuadrantes[3]>=20&&sensores.posF>=lado_cuadrante&&sensores.posC<lado_cuadrante){
					giro45dcha=1;
					accion = TURN_SR;
					cuadrantes[3]=0;
				}else{
					giro45Izq = 3; //haceos giro 180º
					accion = TURN_SR;
				}
		
				break;
			case 5:
				giro45Izq = 6;
				accion = TURN_SR;
				andar=1;
				break;
			case 6:
				accion = TURN_SR;
				andar=1;
				break;
			}
		}

	last_action = accion;
	return accion;

}

void AnularMatrizA(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoAEstrella &st, const vector<Action> &plan)
{
	AnularMatrizA(mapaConPlan);
	EstadoAEstrella cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
			case WALK:
				switch (cst.brujula)
				{
					case 0:
					cst.f--;
					break;
					case 1:
					cst.f--;
					cst.c++;
					break;
					case 2:
					cst.c++;
					break;
					case 3:
					cst.f++;
					cst.c++;
					break;
					case 4:
					cst.f++;
					break;
					case 5:
					cst.f++;
					cst.c--;
					break;
					case 6:
					cst.c--;
					break;
					case 7:
					cst.f--;
					cst.c--;
					break;
				}
				mapaConPlan[cst.f][cst.c] = 2;
				break;
			case TURN_SR:
				cst.brujula = (cst.brujula + 1) % 8;
				break;
		}
		it++;
	}
}

EstadoAEstrella NextCasillaAuxiliar(const EstadoAEstrella &st){
	EstadoAEstrella siguiente = st;
	switch (st.brujula)
	{
		case norte:
			siguiente.f = st.f - 1;
			break;
		case noreste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c + 1;
			break;
		case este:
			siguiente.c = st.c + 1;
			break;
		case sureste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c + 1;
			break;
		case sur:
			siguiente.f = st.f + 1;
			break;
		case suroeste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c - 1;
			break;
		case oeste:
			siguiente.c = st.c - 1;
			break;
		case noroeste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c - 1;
	}
	return siguiente;
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const EstadoAEstrella &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoAEstrella next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;
	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
	return check1 and check2 and check3;
	
}

EstadoAEstrella ComportamientoAuxiliar::applyA(Action accion, const EstadoAEstrella & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoAEstrella next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleAuxiliar(st,terreno,altura)){
				next = NextCasillaAuxiliar(st);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula+1)%8;
		break;
	}
	return next;
}

bool Find (const NodoA & st, const list<NodoA> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !((*it) == st)){
		it++;
	}
	return (it != lista.end());
}

void ComportamientoAuxiliar::PintaPlan(const vector<Action> &plan, bool zap)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == WALK)
		{
			cout << "W ";
		}
		else if (*it == TURN_SR)
		{
			cout << "r ";
		}
		else if (*it == IDLE)
		{
		cout << "I ";
		}
		else
		{
		cout << "-_ ";
		}
		it++;
	}
	cout << "( longitud " << plan.size();
	if (zap) cout << "[Z]";
	cout << ")\n";
}

int ComportamientoAuxiliar::fixBateria_sig_accion_jugador(unsigned char celdaJugador, int difAltura, Action accion)
{
  int bateria_sig_accion = 1;
  switch (accion)
  {
  case IDLE:

  case CALL_ON:
  case CALL_OFF:
    bateria_sig_accion = 0;
    break;
  case WALK:
    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 100;
      if (difAltura > 0)
        bateria_sig_accion += 10;
      else if (difAltura < 0)
        bateria_sig_accion -= 10;
      break;
    case 'T':
      bateria_sig_accion = 20;
      if (difAltura > 0)
        bateria_sig_accion += 5;
      else if (difAltura < 0)
        bateria_sig_accion -= 5;
      break;
    case 'S':
      bateria_sig_accion = 2;
      if (difAltura > 0)
        bateria_sig_accion += 1;
      else if (difAltura < 0)
        bateria_sig_accion -= 1;
      break;
    } // Fin switch celdaJugador
    break;
  case RUN:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 150;
      if (difAltura > 0)
        bateria_sig_accion += 15;
      else if (difAltura < 0)
        bateria_sig_accion -= 15;
      break;
    case 'T':
      bateria_sig_accion = 35;
      if (difAltura > 0)
        bateria_sig_accion += 5;
      else if (difAltura < 0)
        bateria_sig_accion -= 5;
      break;
    case 'S':
      bateria_sig_accion = 3;
      if (difAltura > 0)
        bateria_sig_accion += 2;
      else if (difAltura < 0)
        bateria_sig_accion -= 2;
      break;
    } // Fin switch celdaColaborador
    break;
  case TURN_L:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 30;
      break;
    case 'T':
      bateria_sig_accion = 5;
      break;
    case 'S':
      bateria_sig_accion = 1;
      break;
      break;
    } // Fin switch celdaJugador
    break;
  case TURN_SR:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 16;
      break;
    case 'T':
      bateria_sig_accion = 3;
      break;
    } // Fin switch celdaJugador
    break;
  }

  return bateria_sig_accion;
}

int heuristica(const EstadoAEstrella &actual, const EstadoAEstrella &objetivo) {
    int dist = max(abs(actual.f - objetivo.f), abs(actual.c - objetivo.c));
	return dist;
}


vector<Action> ComportamientoAuxiliar::AEstrella(const EstadoAEstrella &inicio, const EstadoAEstrella &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura) 
{
	priority_queue<NodoAEstrella> frontier;
	set<EstadoAEstrella> explorados;
	vector<Action> plan;
	
	//std::cout << "Objetivo: " << final.f << "," << final.c << std::endl;

	NodoAEstrella current_node;
	current_node.estado = inicio;
	current_node.coste = 0;

	if (terreno[inicio.f][inicio.c] == 'D') {
		current_node.estado.zapatillas = true;
	}
	frontier.push(current_node);
	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
	while (!SolutionFound and !frontier.empty()) {
		// frontier.pop();
		//current_node = frontier.top();
		// std::cout << "Expandiendo: " << current_node.estado.f << "," << current_node.estado.c
        // << " brujula: " << current_node.estado.brujula << " z: " << current_node.estado.zapatillas << std::endl;

		frontier.pop();


		// Objetivo alcanzado
		if (current_node.estado.f == final.f && current_node.estado.c == final.c) {
			SolutionFound = true;
		}
		explorados.insert(current_node.estado);

		// Compruebo si estoy en una casilla que de las zapatillas
		if (terreno[current_node.estado.f][current_node.estado.c] == 'D') {
			current_node.estado.zapatillas = true;
		}


		if(!SolutionFound){
			// Expandir hijos
			//WALK
			EstadoAEstrella siguiente = NextCasillaAuxiliar(current_node.estado);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && CasillaAccesibleAuxiliar(current_node.estado, terreno, altura)){
				NodoAEstrella child_forward = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_forward.coste+=fixBateria_sig_accion_jugador(celda, difAltura, WALK);
				child_forward.estado = applyA(WALK, current_node.estado, terreno, altura);
				child_forward.estimacion = child_forward.coste + heuristica(child_forward.estado, final);
				if (explorados.find(child_forward.estado) == explorados.end()){
					child_forward.secuencia.push_back(WALK);
					frontier.push(child_forward);
				}
			}
			//TURN_SR
			NodoAEstrella child_turnSr = current_node;
			unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnSr.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_SR);
			child_turnSr.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
			child_turnSr.estimacion = child_turnSr.coste + heuristica(child_turnSr.estado, final);
			if (explorados.find(child_turnSr.estado) == explorados.end()){
				child_turnSr.secuencia.push_back(TURN_SR);
				frontier.push(child_turnSr);
			}
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.top();
			SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
			while(!frontier.empty() and explorados.find(current_node.estado) != explorados.end()){
				frontier.pop();
				if(!frontier.empty())
				current_node = frontier.top();

			}
		}
		
	}

	if(SolutionFound){
		plan = current_node.secuencia;
	}

	return plan; 
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan) {
		// Invocar al método de búsqueda
		EstadoAEstrella inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;

		plan = AEstrella(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);

		planIndex = 0; 
		hayPlan = !plan.empty();
	}

	if (hayPlan && planIndex < plan.size()) {
		accion = plan[planIndex];
		planIndex++;
	}

	if (planIndex >= plan.size()) {
		hayPlan = false;
	}

	return accion;
}


Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliarN4(const EstadoAEstrella &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoAEstrella next = NextCasillaAuxiliar(st);
	if(next.f>=0&&next.f<tamanio_mapa&&next.c>=0&&next.c<tamanio_mapa){
		if(terreno[next.f][next.c] != '?'){
			bool check1 = false, check2 = false, check3 = false;
			check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
			check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas);
			check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
			return check1 and check2 and check3;
		}else{
			return true;
		}
	}else{
		return false;
	}
	
}

EstadoAEstrella ComportamientoAuxiliar::applyAN4(Action accion, const EstadoAEstrella & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoAEstrella next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleAuxiliarN4(st,terreno,altura)){
				next = NextCasillaAuxiliar(st);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula+1)%8;
		break;
	}
	return next;
}

vector<Action> ComportamientoAuxiliar::AEstrellaN4(const EstadoAEstrella &inicio, const EstadoAEstrella &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura) 
{
	priority_queue<NodoAEstrella> frontier;
	set<EstadoAEstrella> explorados;
	vector<Action> plan;
	
	//std::cout << "Objetivo: " << final.f << "," << final.c << std::endl;

	NodoAEstrella current_node;
	current_node.estado = inicio;
	current_node.coste = 0;

	if (terreno[inicio.f][inicio.c] == 'D') {
		current_node.estado.zapatillas = true;
	}
	frontier.push(current_node);
	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
	while (!SolutionFound and !frontier.empty()) {
		// frontier.pop();
		//current_node = frontier.top();
		// std::cout << "Expandiendo: " << current_node.estado.f << "," << current_node.estado.c
        // << " brujula: " << current_node.estado.brujula << " z: " << current_node.estado.zapatillas << std::endl;

		frontier.pop();


		// Objetivo alcanzado
		if (current_node.estado.f == final.f && current_node.estado.c == final.c) {
			SolutionFound = true;
		}
		explorados.insert(current_node.estado);

		// Compruebo si estoy en una casilla que de las zapatillas
		if (terreno[current_node.estado.f][current_node.estado.c] == 'D') {
			current_node.estado.zapatillas = true;
		}


		if(!SolutionFound){
			// Expandir hijos
			//WALK
			EstadoAEstrella siguiente = NextCasillaAuxiliar(current_node.estado);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && CasillaAccesibleAuxiliarN4(current_node.estado, terreno, altura)){
				NodoAEstrella child_forward = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_forward.coste+=fixBateria_sig_accion_jugador(celda, difAltura, WALK);
				child_forward.estado = applyAN4(WALK, current_node.estado, terreno, altura);
				child_forward.estimacion = child_forward.coste + heuristica(child_forward.estado, final);
				if (explorados.find(child_forward.estado) == explorados.end()){
					child_forward.secuencia.push_back(WALK);
					frontier.push(child_forward);
				}
			}
			//TURN_SR
			NodoAEstrella child_turnSr = current_node;
			unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnSr.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_SR);
			child_turnSr.estado = applyAN4(TURN_SR, current_node.estado, terreno, altura);
			child_turnSr.estimacion = child_turnSr.coste + heuristica(child_turnSr.estado, final);
			if (explorados.find(child_turnSr.estado) == explorados.end()){
				child_turnSr.secuencia.push_back(TURN_SR);
				frontier.push(child_turnSr);
			}
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.top();
			SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
			while(!frontier.empty() and explorados.find(current_node.estado) != explorados.end()){
				frontier.pop();
				if(!frontier.empty())
				current_node = frontier.top();

			}
		}
		
	}

	if(SolutionFound){
		plan = current_node.secuencia;
	}

	return plan; 
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D'){
		tiene_zapatillas = true;
	}
	if(sensores.superficie[0] == 'X' and sensores.energia < 2000 && instante_simulacion-sensores.energia > 0){
		instante_simulacion--;
		last_action = IDLE;
		return IDLE;
	}
	bool objetivo_avistado=false;
	// if(sensores.destinoF!=-1 and sensores.destinoC!=-1){
	// 	for (int i = 1; i <= 8; i++) {
	// 		if (sensores.agentes[i] == 'r') {
	// 			objetivo_avistado = true;
	// 			break;
	// 		}
	// 	}
	// }
	

	Action accion = IDLE;

	if (!hayPlan and (sensores.destinoF!=-1 and sensores.destinoC!=-1)) {
		// Invocar al método de búsqueda
		EstadoAEstrella inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;

		plan = AEstrellaN4(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);

		planIndex = 0; 
		hayPlan = !plan.empty();
	}

	if (hayPlan && planIndex < plan.size() && !objetivo_avistado) {
		accion = plan[planIndex];
		planIndex++;

		if(accion==WALK){
			EstadoAEstrella aux;
			aux.f = sensores.posF;
			aux.c = sensores.posC;
			aux.brujula = sensores.rumbo;
			aux.zapatillas = tiene_zapatillas;
			if(!CasillaAccesibleAuxiliarN4(aux, mapaResultado, mapaCotas)){
				accion = IDLE;
				plan.clear();
			}
		}
		
	}
	if(objetivo_avistado){
		plan.clear();
	}
	if (planIndex >= plan.size()) {
		hayPlan = false;
	}

	return accion;
}

// list<Action> AvanzaASaltosDeCaballo(){
// 	list<Action> secuencia;
// 	secuencia.push_back(WALK);
// 	secuencia.push_back(WALK);
// 	secuencia.push_back(TURN_SR);
// 	secuencia.push_back(TURN_SR);
// 	secuencia.push_back(WALK);
// 	return secuencia;
// }


// list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
// 	NodoA current_node;
// 	list<NodoA> frontier;
// 	list<NodoA> explored;
// 	list<Action> path;

// 	current_node.estado=inicio;
// 	frontier.push_back(current_node);
// 	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
// 	while(!SolutionFound and !frontier.empty()){
// 		frontier.pop_front();
// 		explored.push_back(current_node);

// 		// Compruebo si estoy en una casilla que de las zapatillas
// 		if (terreno[current_node.estado.f][current_node.estado.c] == 'D') {
// 			current_node.estado.zapatillas = true;
// 		}

// 		// Genero el hijo resultante de aplicar la acción WALK
// 		NodoA child_WALK = current_node;
// 		child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
// 		if (child_WALK.estado.f == final.f && child_WALK.estado.c == final.c) {
// 			// El hijo generado es solución
// 			child_WALK.secuencia.push_back(WALK);
// 			current_node = child_WALK;
// 			SolutionFound = true;
// 		} else if (!Find(child_WALK,frontier)and!Find(child_WALK,explored)) {
// 			// Se mete en la lista frontier después de añadir la secuencia la acción
// 			child_WALK.secuencia.push_back(WALK);
// 			frontier.push_back(child_WALK);
// 		}

// 		// Genero el hijo resultante de aplicar la acción TURN_SR
// 		if (!SolutionFound) {
// 			NodoA child_TURN_SR = current_node;
// 			child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
// 			if (!Find(child_TURN_SR,frontier)and!Find(child_TURN_SR,explored)) {
// 				child_TURN_SR.secuencia.push_back(TURN_SR);
// 				frontier.push_back(child_TURN_SR);
// 			}
// 		}

// 		// Paso a evaluar el siguiente nodo en la lista "frontier"
// 		if (!SolutionFound && !frontier.empty()) {
// 			current_node = frontier.front();
// 			SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
// 		}
// 	}

// 	if(SolutionFound) path = current_node.secuencia;

// 	return path;
// }
// list<Action> ComportamientoAuxiliar::AnchuraAuxiliar_V2(const EstadoA &inicio, const EstadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
// 	NodoA current_node;
// 	list<NodoA> frontier;
// 	set<NodoA> explored;
// 	list<Action> path;

// 	current_node.estado=inicio;
// 	frontier.push_back(current_node);
// 	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
// 	while(!SolutionFound and !frontier.empty()){
// 		frontier.pop_front();
// 		explored.insert(current_node);

// 		// Compruebo si estoy en una casilla que de las zapatillas
// 		if (terreno[current_node.estado.f][current_node.estado.c] == 'D') {
// 			current_node.estado.zapatillas = true;
// 		}

// 		// Genero el hijo resultante de aplicar la acción WALK
// 		NodoA child_WALK = current_node;
// 		child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
// 		if (child_WALK.estado.f == final.f && child_WALK.estado.c == final.c) {
// 			// El hijo generado es solución
// 			child_WALK.secuencia.push_back(WALK);
// 			current_node = child_WALK;
// 			SolutionFound = true;
// 		} else if (explored.find(child_WALK) == explored.end()) {
// 			// Se mete en la lista frontier después de añadir la secuencia la acción
// 			child_WALK.secuencia.push_back(WALK);
// 			frontier.push_back(child_WALK);
// 		}

// 		// Genero el hijo resultante de aplicar la acción TURN_SR
// 		if (!SolutionFound) {
// 			NodoA child_TURN_SR = current_node;
// 			child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
// 			if (explored.find(child_TURN_SR) == explored.end()) {
// 				child_TURN_SR.secuencia.push_back(TURN_SR);
// 				frontier.push_back(child_TURN_SR);
// 			}
// 		}

// 		// Paso a evaluar el siguiente nodo en la lista "frontier"
// 		if (!SolutionFound && !frontier.empty()) {
// 			current_node = frontier.front();
// 			while (explored.find(current_node) != explored.end() && !frontier.empty()) {
// 				frontier.pop_front();
// 				current_node = frontier.front();
// 			}
// 		}
// 	}

// 	if(SolutionFound) path = current_node.secuencia;

// 	return path;
// }

// Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores)
// {
// 	Action accion = IDLE;
// 	if (!hayPlan){
// 		// Invocar al método de búsqueda
// 		EstadoA inicio, fin;
// 		inicio.f = sensores.posF;
// 		inicio.c = sensores.posC;
// 		inicio.brujula = sensores.rumbo;
// 		inicio.zapatillas = tiene_zapatillas;
// 		fin.f = sensores.destinoF;
// 		fin.c = sensores.destinoC;
// 		plan = AnchuraAuxiliar_V2(inicio, fin, mapaResultado, mapaCotas);
// 		VisualizaPlan(inicio,plan);
// 		hayPlan = plan.size() != 0 ;
// 	}
// 	if (hayPlan and plan.size()>0){
// 		accion = plan.front();
// 		plan.pop_front();
// 	}
// 	if (plan.size()== 0){
// 		hayPlan = false;
// 	}
// 	return accion;
// }

void ComportamientoAuxiliar::SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores){

	m[sensores.posF][sensores.posC] = sensores.superficie[0];
	a[sensores.posF][sensores.posC] = sensores.cota[0];

	int pos=1;
	switch (sensores.rumbo)
	{
	case norte:
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[1];
		m[sensores.posF-1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[3];
		m[sensores.posF-2][sensores.posC-2] = sensores.superficie[4];
		m[sensores.posF-2][sensores.posC-1] = sensores.superficie[5];
		m[sensores.posF-2][sensores.posC] = sensores.superficie[6];
		m[sensores.posF-2][sensores.posC+1] = sensores.superficie[7];
		m[sensores.posF-2][sensores.posC+2] = sensores.superficie[8];
		m[sensores.posF-3][sensores.posC-3] = sensores.superficie[9];
		m[sensores.posF-3][sensores.posC-2] = sensores.superficie[10];
		m[sensores.posF-3][sensores.posC-1] = sensores.superficie[11];
		m[sensores.posF-3][sensores.posC] = sensores.superficie[12];
		m[sensores.posF-3][sensores.posC+1] = sensores.superficie[13];
		m[sensores.posF-3][sensores.posC+2] = sensores.superficie[14];
		m[sensores.posF-3][sensores.posC+3] = sensores.superficie[15];

		a[sensores.posF-1][sensores.posC-1] = sensores.cota[1];
		a[sensores.posF-1][sensores.posC] = sensores.cota[2];
		a[sensores.posF-1][sensores.posC+1] = sensores.cota[3];
		a[sensores.posF-2][sensores.posC-2] = sensores.cota[4];
		a[sensores.posF-2][sensores.posC-1] = sensores.cota[5];
		a[sensores.posF-2][sensores.posC] = sensores.cota[6];
		a[sensores.posF-2][sensores.posC+1] = sensores.cota[7];
		a[sensores.posF-2][sensores.posC+2] = sensores.cota[8];
		a[sensores.posF-3][sensores.posC-3] = sensores.cota[9];
		a[sensores.posF-3][sensores.posC-2] = sensores.cota[10];
		a[sensores.posF-3][sensores.posC-1] = sensores.cota[11];
		a[sensores.posF-3][sensores.posC] = sensores.cota[12];
		a[sensores.posF-3][sensores.posC+1] = sensores.cota[13];
		a[sensores.posF-3][sensores.posC+2] = sensores.cota[14];
		a[sensores.posF-3][sensores.posC+3] = sensores.cota[15];
		
		break;
	case noreste:
		m[sensores.posF-1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC+1] = sensores.superficie[3];
		m[sensores.posF-2][sensores.posC] = sensores.superficie[4];
		m[sensores.posF-2][sensores.posC+1] = sensores.superficie[5];
		m[sensores.posF-2][sensores.posC+2] = sensores.superficie[6];
		m[sensores.posF-1][sensores.posC+2] = sensores.superficie[7];
		m[sensores.posF][sensores.posC+2] = sensores.superficie[8];
		m[sensores.posF-3][sensores.posC] = sensores.superficie[9];
		m[sensores.posF-3][sensores.posC+1] = sensores.superficie[10];
		m[sensores.posF-3][sensores.posC+2] = sensores.superficie[11];
		m[sensores.posF-3][sensores.posC+3] = sensores.superficie[12];
		m[sensores.posF-2][sensores.posC+3] = sensores.superficie[13];
		m[sensores.posF-1][sensores.posC+3] = sensores.superficie[14];
		m[sensores.posF][sensores.posC+3] = sensores.superficie[15];

		a[sensores.posF-1][sensores.posC] = sensores.cota[1];
		a[sensores.posF-1][sensores.posC+1] = sensores.cota[2];
		a[sensores.posF][sensores.posC+1] = sensores.cota[3];
		a[sensores.posF-2][sensores.posC] = sensores.cota[4];
		a[sensores.posF-2][sensores.posC+1] = sensores.cota[5];
		a[sensores.posF-2][sensores.posC+2] = sensores.cota[6];
		a[sensores.posF-1][sensores.posC+2] = sensores.cota[7];
		a[sensores.posF][sensores.posC+2] = sensores.cota[8];
		a[sensores.posF-3][sensores.posC] = sensores.cota[9];
		a[sensores.posF-3][sensores.posC+1] = sensores.cota[10];
		a[sensores.posF-3][sensores.posC+2] = sensores.cota[11];
		a[sensores.posF-3][sensores.posC+3] = sensores.cota[12];
		a[sensores.posF-2][sensores.posC+3] = sensores.cota[13];
		a[sensores.posF-1][sensores.posC+3] = sensores.cota[14];
		a[sensores.posF][sensores.posC+3] = sensores.cota[15];
		break;
	case este:
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
		m[sensores.posF-2][sensores.posC+2] = sensores.superficie[4];
		m[sensores.posF-1][sensores.posC+2] = sensores.superficie[5];
		m[sensores.posF][sensores.posC+2] = sensores.superficie[6];
		m[sensores.posF+1][sensores.posC+2] = sensores.superficie[7];
		m[sensores.posF+2][sensores.posC+2] = sensores.superficie[8];
		m[sensores.posF-3][sensores.posC+3] = sensores.superficie[9];
		m[sensores.posF-2][sensores.posC+3] = sensores.superficie[10];
		m[sensores.posF-1][sensores.posC+3] = sensores.superficie[11];
		m[sensores.posF][sensores.posC+3] = sensores.superficie[12];
		m[sensores.posF+1][sensores.posC+3] = sensores.superficie[13];
		m[sensores.posF+2][sensores.posC+3] = sensores.superficie[14];
		m[sensores.posF+3][sensores.posC+3] = sensores.superficie[15];

		a[sensores.posF-1][sensores.posC+1] = sensores.cota[1];
		a[sensores.posF][sensores.posC+1] = sensores.cota[2];
		a[sensores.posF+1][sensores.posC+1] = sensores.cota[3];
		a[sensores.posF-2][sensores.posC+2] = sensores.cota[4];
		a[sensores.posF-1][sensores.posC+2] = sensores.cota[5];
		a[sensores.posF][sensores.posC+2] = sensores.cota[6];
		a[sensores.posF+1][sensores.posC+2] = sensores.cota[7];
		a[sensores.posF+2][sensores.posC+2] = sensores.cota[8];
		a[sensores.posF-3][sensores.posC+3] = sensores.cota[9];
		a[sensores.posF-2][sensores.posC+3] = sensores.cota[10];
		a[sensores.posF-1][sensores.posC+3] = sensores.cota[11];
		a[sensores.posF][sensores.posC+3] = sensores.cota[12];
		a[sensores.posF+1][sensores.posC+3] = sensores.cota[13];
		a[sensores.posF+2][sensores.posC+3] = sensores.cota[14];
		a[sensores.posF+3][sensores.posC+3] = sensores.cota[15];
		break;
	case sureste:
		m[sensores.posF+1][sensores.posC] = sensores.superficie[3];
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC+1] = sensores.superficie[1];
		m[sensores.posF+2][sensores.posC] = sensores.superficie[8];
		m[sensores.posF+2][sensores.posC+1] = sensores.superficie[7];
		m[sensores.posF+2][sensores.posC+2] = sensores.superficie[6];
		m[sensores.posF+1][sensores.posC+2] = sensores.superficie[5];
		m[sensores.posF][sensores.posC+2] = sensores.superficie[4];
		m[sensores.posF+3][sensores.posC] = sensores.superficie[15];
		m[sensores.posF+3][sensores.posC+1] = sensores.superficie[14];
		m[sensores.posF+3][sensores.posC+2] = sensores.superficie[13];
		m[sensores.posF+3][sensores.posC+3] = sensores.superficie[12];
		m[sensores.posF+2][sensores.posC+3] = sensores.superficie[11];
		m[sensores.posF+1][sensores.posC+3] = sensores.superficie[10];
		m[sensores.posF][sensores.posC+3] = sensores.superficie[9];

		a[sensores.posF+1][sensores.posC] = sensores.cota[3];
		a[sensores.posF+1][sensores.posC+1] = sensores.cota[2];
		a[sensores.posF][sensores.posC+1] = sensores.cota[1];
		a[sensores.posF+2][sensores.posC] = sensores.cota[8];
		a[sensores.posF+2][sensores.posC+1] = sensores.cota[7];
		a[sensores.posF+2][sensores.posC+2] = sensores.cota[6];
		a[sensores.posF+1][sensores.posC+2] = sensores.cota[5];
		a[sensores.posF][sensores.posC+2] = sensores.cota[4];
		a[sensores.posF+3][sensores.posC] = sensores.cota[15];
		a[sensores.posF+3][sensores.posC+1] = sensores.cota[14];
		a[sensores.posF+3][sensores.posC+2] = sensores.cota[13];
		a[sensores.posF+3][sensores.posC+3] = sensores.cota[12];
		a[sensores.posF+2][sensores.posC+3] = sensores.cota[11];
		a[sensores.posF+1][sensores.posC+3] = sensores.cota[10];
		a[sensores.posF][sensores.posC+3] = sensores.cota[9];
		break;
	case sur:
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[1];
		m[sensores.posF+1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[3];
		m[sensores.posF+2][sensores.posC+2] = sensores.superficie[4];
		m[sensores.posF+2][sensores.posC+1] = sensores.superficie[5];
		m[sensores.posF+2][sensores.posC] = sensores.superficie[6];
		m[sensores.posF+2][sensores.posC-1] = sensores.superficie[7];
		m[sensores.posF+2][sensores.posC-2] = sensores.superficie[8];
		m[sensores.posF+3][sensores.posC+3] = sensores.superficie[9];
		m[sensores.posF+3][sensores.posC+2] = sensores.superficie[10];
		m[sensores.posF+3][sensores.posC+1] = sensores.superficie[11];
		m[sensores.posF+3][sensores.posC] = sensores.superficie[12];
		m[sensores.posF+3][sensores.posC-1] = sensores.superficie[13];
		m[sensores.posF+3][sensores.posC-2] = sensores.superficie[14];
		m[sensores.posF+3][sensores.posC-3] = sensores.superficie[15];

		a[sensores.posF+1][sensores.posC+1] = sensores.cota[1];
		a[sensores.posF+1][sensores.posC] = sensores.cota[2];
		a[sensores.posF+1][sensores.posC-1] = sensores.cota[3];
		a[sensores.posF+2][sensores.posC+2] = sensores.cota[4];
		a[sensores.posF+2][sensores.posC+1] = sensores.cota[5];
		a[sensores.posF+2][sensores.posC] = sensores.cota[6];
		a[sensores.posF+2][sensores.posC-1] = sensores.cota[7];
		a[sensores.posF+2][sensores.posC-2] = sensores.cota[8];
		a[sensores.posF+3][sensores.posC+3] = sensores.cota[9];
		a[sensores.posF+3][sensores.posC+2] = sensores.cota[10];
		a[sensores.posF+3][sensores.posC+1] = sensores.cota[11];
		a[sensores.posF+3][sensores.posC] = sensores.cota[12];
		a[sensores.posF+3][sensores.posC-1] = sensores.cota[13];
		a[sensores.posF+3][sensores.posC-2] = sensores.cota[14];
		a[sensores.posF+3][sensores.posC-3] = sensores.cota[15];
		break;
	case suroeste:
		m[sensores.posF+1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC-1] = sensores.superficie[3];
		m[sensores.posF+2][sensores.posC] = sensores.superficie[4];
		m[sensores.posF+2][sensores.posC-1] = sensores.superficie[5];
		m[sensores.posF+2][sensores.posC-2] = sensores.superficie[6];
		m[sensores.posF+1][sensores.posC-2] = sensores.superficie[7];
		m[sensores.posF][sensores.posC-2] = sensores.superficie[8];
		m[sensores.posF+3][sensores.posC] = sensores.superficie[9];
		m[sensores.posF+3][sensores.posC-1] = sensores.superficie[10];
		m[sensores.posF+3][sensores.posC-2] = sensores.superficie[11];
		m[sensores.posF+3][sensores.posC-3] = sensores.superficie[12];
		m[sensores.posF+2][sensores.posC-3] = sensores.superficie[13];
		m[sensores.posF+1][sensores.posC-3] = sensores.superficie[14];
		m[sensores.posF][sensores.posC-3] = sensores.superficie[15];

		a[sensores.posF+1][sensores.posC] = sensores.cota[1];
		a[sensores.posF+1][sensores.posC-1] = sensores.cota[2];
		a[sensores.posF][sensores.posC-1] = sensores.cota[3];
		a[sensores.posF+2][sensores.posC] = sensores.cota[4];
		a[sensores.posF+2][sensores.posC-1] = sensores.cota[5];
		a[sensores.posF+2][sensores.posC-2] = sensores.cota[6];
		a[sensores.posF+1][sensores.posC-2] = sensores.cota[7];
		a[sensores.posF][sensores.posC-2] = sensores.cota[8];
		a[sensores.posF+3][sensores.posC] = sensores.cota[9];
		a[sensores.posF+3][sensores.posC-1] = sensores.cota[10];
		a[sensores.posF+3][sensores.posC-2] = sensores.cota[11];
		a[sensores.posF+3][sensores.posC-3] = sensores.cota[12];
		a[sensores.posF+2][sensores.posC-3] = sensores.cota[13];
		a[sensores.posF+1][sensores.posC-3] = sensores.cota[14];
		a[sensores.posF][sensores.posC-3] = sensores.cota[15];
		break;
	case oeste:
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC-1] = sensores.superficie[2];
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[3];
		m[sensores.posF+2][sensores.posC-2] = sensores.superficie[4];
		m[sensores.posF+1][sensores.posC-2] = sensores.superficie[5];
		m[sensores.posF][sensores.posC-2] = sensores.superficie[6];
		m[sensores.posF-1][sensores.posC-2] = sensores.superficie[7];
		m[sensores.posF-2][sensores.posC-2] = sensores.superficie[8];
		m[sensores.posF+3][sensores.posC-3] = sensores.superficie[9];
		m[sensores.posF+2][sensores.posC-3] = sensores.superficie[10];
		m[sensores.posF+1][sensores.posC-3] = sensores.superficie[11];
		m[sensores.posF][sensores.posC-3] = sensores.superficie[12];
		m[sensores.posF-1][sensores.posC-3] = sensores.superficie[13];
		m[sensores.posF-2][sensores.posC-3] = sensores.superficie[14];
		m[sensores.posF-3][sensores.posC-3] = sensores.superficie[15];

		a[sensores.posF+1][sensores.posC-1] = sensores.cota[1];
		a[sensores.posF][sensores.posC-1] = sensores.cota[2];
		a[sensores.posF-1][sensores.posC-1] = sensores.cota[3];
		a[sensores.posF+2][sensores.posC-2] = sensores.cota[4];
		a[sensores.posF+1][sensores.posC-2] = sensores.cota[5];
		a[sensores.posF][sensores.posC-2] = sensores.cota[6];
		a[sensores.posF-1][sensores.posC-2] = sensores.cota[7];
		a[sensores.posF-2][sensores.posC-2] = sensores.cota[8];
		a[sensores.posF+3][sensores.posC-3] = sensores.cota[9];
		a[sensores.posF+2][sensores.posC-3] = sensores.cota[10];
		a[sensores.posF+1][sensores.posC-3] = sensores.cota[11];
		a[sensores.posF][sensores.posC-3] = sensores.cota[12];
		a[sensores.posF-1][sensores.posC-3] = sensores.cota[13];
		a[sensores.posF-2][sensores.posC-3] = sensores.cota[14];
		a[sensores.posF-3][sensores.posC-3] = sensores.cota[15];
		break;
	case noroeste:
		m[sensores.posF-1][sensores.posC] = sensores.superficie[3];
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC-1] = sensores.superficie[1];
		m[sensores.posF-2][sensores.posC] = sensores.superficie[8];
		m[sensores.posF-2][sensores.posC-1] = sensores.superficie[7];
		m[sensores.posF-2][sensores.posC-2] = sensores.superficie[6];
		m[sensores.posF-1][sensores.posC-2] = sensores.superficie[5];
		m[sensores.posF][sensores.posC-2] = sensores.superficie[4];
		m[sensores.posF-3][sensores.posC] = sensores.superficie[15];
		m[sensores.posF-3][sensores.posC-1] = sensores.superficie[14];
		m[sensores.posF-3][sensores.posC-2] = sensores.superficie[13];
		m[sensores.posF-3][sensores.posC-3] = sensores.superficie[12];
		m[sensores.posF-2][sensores.posC-3] = sensores.superficie[11];
		m[sensores.posF-1][sensores.posC-3] = sensores.superficie[10];
		m[sensores.posF][sensores.posC-3] = sensores.superficie[9];

		a[sensores.posF-1][sensores.posC] = sensores.cota[3];
		a[sensores.posF-1][sensores.posC-1] = sensores.cota[2];
		a[sensores.posF][sensores.posC-1] = sensores.cota[1];
		a[sensores.posF-2][sensores.posC] = sensores.cota[8];
		a[sensores.posF-2][sensores.posC-1] = sensores.cota[7];
		a[sensores.posF-2][sensores.posC-2] = sensores.cota[6];
		a[sensores.posF-1][sensores.posC-2] = sensores.cota[5];
		a[sensores.posF][sensores.posC-2] = sensores.cota[4];
		a[sensores.posF-3][sensores.posC] = sensores.cota[15];
		a[sensores.posF-3][sensores.posC-1] = sensores.cota[14];
		a[sensores.posF-3][sensores.posC-2] = sensores.cota[13];
		a[sensores.posF-3][sensores.posC-3] = sensores.cota[12];
		a[sensores.posF-2][sensores.posC-3] = sensores.cota[11];
		a[sensores.posF-1][sensores.posC-3] = sensores.cota[10];
		a[sensores.posF][sensores.posC-3] = sensores.cota[9];
		break;
	
	default:
		break;
	}

}