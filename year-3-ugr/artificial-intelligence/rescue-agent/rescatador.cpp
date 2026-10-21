#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"

//2 fases:
//1) Una primera fase de obervacion donde se actualizan los cambios de la ultima accion
//2) Una segunda fase donde se decide la siguiente accion a realizar
Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

char ComportamientoRescatador::ViablePorAlturaR(char casilla, int dif, bool zap){
	if(abs(dif)<= 1 or (zap and abs(dif)<= 2)){
		return casilla;
	}else{
		return 'P';
	}
}

pair<int, int> posDelanteR(Orientacion o, pair<int,int> posActual){
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
pair<int, int> posIzqR(Orientacion o, pair<int,int> posActual){
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
pair<int, int> posDchaR(Orientacion o,  pair<int,int> posActual){
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

int ComportamientoRescatador::VeoCasillaIntersanteR(char i, char c, char d, bool zap, char a,char a_i, char a_d,  char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual){

	// if (c == 'X' && visitasPorCasilla[posDelanteR(orientacion, posActual)] == 0 && a != 'a') return 2;
    // if (i == 'X' && visitasPorCasilla[posIzqR(orientacion, posActual)] == 0 && a_i != 'a') return 1;
    // if (d == 'X' && visitasPorCasilla[posDchaR(orientacion, posActual)] == 0 && a_d != 'a') return 3;
	// if (c == 'C' && visitasPorCasilla[posDelanteR(orientacion, posActual)] == 0 && a != 'a') return 2;
	// if (i == 'C'  && visitasPorCasilla[posIzqR(orientacion, posActual)] == 0 && a_i != 'a') return 1;
	// if (d == 'C'  && visitasPorCasilla[posDchaR(orientacion, posActual)] == 0 && a_d != 'a') return 3;
	
	if(c=='X' && a!='a') return 2;
	else if(i=='X' && a_i!='a') return 1;
	else if(d=='X'&& a_d!='a') return 3;
	else if(!zap){
		if(c == 'D' && a!='a') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}
	if(c=='C'&& a!='a'){
		if(orientacion==sureste||orientacion==noreste||orientacion==suroeste||orientacion==noroeste){
			return 2;
		}else if(c6=='C'||c6=='X'){
			return 2;
		}else if(i=='C'){
			return 4;
		}else if(d=='C'){
			return 5;
		}else if(i=='P'&&(c5=='C'||c5=='X')){
			return 2;
		}else if(d=='P'&&(c7=='C'||c7=='X')){
			return 2;
		}
	}
	else if(i=='C') return 1;
	else if(d=='C') return 3;
	else return 0;

}

bool Run(  char a, char a_6, char c, char c6){

	bool run = false;
	if(a!='a' && a_6 !='a'&&c!='M'&&c!='P'&&c!='B'&&c!='T'&&(c6=='C'||c6=='S')){
		run=true;
	}

	return run;

}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	//1) Una primera fase de observación donde se actualizan los cambios de la última acción
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D'){
		tiene_zapatillas = true;
	}
	if(sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){ //noroeste
		cuadrantes[0]++;
	} else if(sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){ //noreste
		cuadrantes[1]++;
	} else if(sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){ //sureste
		cuadrantes[2]++;
	} else if(sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){ //suroeste
		cuadrantes[3]++;
	}


	bool auxiliar_en_base = false;
	pair<int, int> posActual = {sensores.posF, sensores.posC};
	//Para el case 0
	char terreno = 'C';  
    std::pair<int, int> frente = {0, 0}; 
	int MAX_GIROS_ALTERNOS = 2;

	//Guardamos la acción, orientación y visitas
	ultimaAccionPorCasilla[posActual] = last_action;
	ultimaOrientacionPorCasilla[posActual] = sensores.rumbo;
	//cout << visitasPorCasilla[posActual] << endl;

	//2) Una segunda fase donde se decide la siguiente acción a realizar
	if(sensores.superficie[0] == 'X'){ //Llegue al objetivo
		accion = IDLE;
	} else if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	} else if(giro180 != 0){
		accion = TURN_L;
		giro180--;
	} else if(giro45dcha != 0){
		accion = TURN_SR;
		giro45dcha--;
	} else if(andar != 0){
		if(sensores.agentes[2]!='a'){
			accion = WALK;
		}else{
			accion = IDLE;
		}
		andar--;
	} else {
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaIntersanteR(i, c, d, tiene_zapatillas, sensores.agentes[2], sensores.agentes[1],sensores.agentes[3],sensores.superficie[5], sensores.superficie[6], sensores.superficie[7], sensores.rumbo, posActual);
		frente = posDelanteR(sensores.rumbo, posActual);
		terreno = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		bool casilla_frente_viable = ((terreno == 'C'||terreno == 'X') && visitasPorCasilla[frente] < 3 && sensores.agentes[2] != 'a');

		if (!casilla_frente_viable &&
			visitasPorCasilla[posActual] > 4 &&
			ultimaOrientacionPorCasilla[posActual] == sensores.rumbo &&
			ultimaAccionPorCasilla[posActual] == WALK &&
			last_action == WALK)
		{
			buclePorCasilla[posActual]++;
			
			if (buclePorCasilla[posActual] >= 2) {
				if (giroAlterno < MAX_GIROS_ALTERNOS) {
					accion = TURN_L;
					giro180 = 1;
					giroAlterno++;
				} else {
					accion = TURN_SR;
					giroAlterno = 0;  
				}
				visitasPorCasilla[posActual] -= 3;
				buclePorCasilla[posActual] = 0;
				last_action = accion;
				visitasPorCasilla[posActual]++;
				return accion;
			} else {
				accion = TURN_L;
				last_action = accion;
				visitasPorCasilla[posActual]++;
				return accion;
			}
		}


		switch (pos)
		{
		case 2:
			accion = WALK;
			for (int i = 1; i <= 8; i++) {
				if (sensores.agentes[i] == 'a') {
					if (sensores.superficie[i] == 'X') {
						auxiliar_en_base = true;
						break;
					}
				}
			}
			switch (sensores.rumbo)
			{
			case sureste:
				if(sensores.agentes[3]=='a'&&!auxiliar_en_base){
					accion = IDLE;
				}
				break;
			case noreste:
				if(sensores.agentes[1]=='a'&&!auxiliar_en_base){
					accion = IDLE;
				}
				break;
			case suroeste:
				if(sensores.agentes[1]=='a'&&!auxiliar_en_base){
					accion = IDLE;
				}
				break;
			case noroeste:
				if(sensores.agentes[3]=='a'&&!auxiliar_en_base){
					accion = IDLE;
				}
				break;
			
			default:
				break;
			}
			// switch (sensores.rumbo){
			// 	case norte:
			// 	case este:
			// 	case sur:
			// 	case oeste:
			// 		accion = WALK;
			// 		if(sensores.agentes[1] == 'a' || sensores.agentes[3] == 'a' || sensores.agentes[5] == 'a' || sensores.agentes[6] == 'a' || sensores.agentes[7] == 'a'){
			// 			if(cuadrantes[0] >= 15 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[0] = 0;
			// 			} else if(cuadrantes[1] >= 15 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[1] = 0;
			// 			} else if(cuadrantes[2] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[2] = 0;
			// 			} else if(cuadrantes[3] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[3] = 0;
			// 			} else {
			// 				accion = TURN_L;
			// 			}
			// 		}
			// 		break;
			// 	case noreste:
			// 	case noroeste:
			// 	case sureste:
			// 	case suroeste:
			// 		accion = WALK;
			// 		if(sensores.agentes[1] == 'a' || sensores.agentes[3] == 'a' || sensores.agentes[4] == 'a' || sensores.agentes[5] == 'a' || sensores.agentes[6] == 'a' || sensores.agentes[7] == 'a' || sensores.agentes[8] == 'a'){
			// 			if(cuadrantes[0] >= 15 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[0] = 0;
			// 			} else if(cuadrantes[1] >= 15 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[1] = 0;
			// 			} else if(cuadrantes[2] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[2] = 0;
			// 			} else if(cuadrantes[3] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[3] = 0;
			// 			} else {
			// 				accion = TURN_L;
			// 			}
			// 		}
			// 		break;
			// }
			break;
		case 1:
			giro45Izq = 1;
			accion = TURN_L;
			break;
		case 3:
			accion = TURN_SR;
			break;
		case 0:  
			frente = posDelanteR(sensores.rumbo, posActual);

			// Verificamos si la casilla adelante es viable y si no se ha visitado muchas veces
			terreno = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
			if ((terreno == 'C'||terreno == 'X') && visitasPorCasilla[frente] < 2 && sensores.agentes[2] != 'a') {
				accion = WALK;  // Camina si la casilla es viable y no la has visitado demasiado
			} else {
				if(cuadrantes[0] >= 20 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[0] = 0;
				} else if(cuadrantes[1] >= 20 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[1] = 0;
				} else if(cuadrantes[2] >= 20 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[2] = 0;
				} else if(cuadrantes[3] >= 20 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[3] = 0;
				} else {
					accion = TURN_L;
					giro180 = 1;
				}
			}
			break;
		case 4:
			giro45Izq = 1;
			accion = TURN_L;
			andar = 1;
			break;
		case 5:
			accion = TURN_SR;
			andar = 1;
			break;
		}
	}

	last_action = accion;
	visitasPorCasilla[posActual]++;
	return accion;
}

int ComportamientoRescatador::VeoCasillaIntersanteR1(char i, char c, char d, bool zap, char a,char a_i, char a_d,  char c5,char c6,char c7, Orientacion orientacion, pair<int,int> posActual, bool run){

	const int MAX_VISITAS = 4;
	if(!zap){
		if(c == 'D' && a!='a') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}
	if(c5!='C'&&c5!='S'&&c7!='C'&&c7!='S'&&run&&visitasPorCasilla[posDelanteR(orientacion,posDelanteR(orientacion, posActual))] < MAX_VISITAS) return 6;
	if((c=='C'||c=='S'||c == 'D'||c=='X')&& a!='a'){
		if((orientacion==sureste||orientacion==noreste||orientacion==suroeste||orientacion==noroeste)&&visitasPorCasilla[posDelanteR(orientacion, posActual)] < MAX_VISITAS){
			return 2;
		}else if((c6=='C'||c6=='S'||c == 'D'||c=='X')&&visitasPorCasilla[posDelanteR(orientacion, posActual)] < MAX_VISITAS){
			return 2;
		}else if(i=='C'||i=='S'){
			return 4;
		}else if(d=='C'||d=='S'){
			return 5;
		}else if(i=='P'&&(c5=='C'||c5=='S')){
			return 2;
		}else if(d=='P'&&(c7=='C'||c7=='S')){
			return 2;
		}
	}
	else if(i=='C'||i=='S') return 1;
	else if(d=='C'||d=='S') return 3;
	else return 0;

    // Prioridades base
    // const int MAX_VISITAS = 3;

    // // 1. Si no tiene zapatillas, buscar 'D' prioritariamente
    // if (!zap) {
    //     if (c == 'D' && a != 'a') return 2;
    //     if (i == 'D') return 1;
    //     if (d == 'D') return 3;
    // }

    // // 2. Explorar hacia delante si es sendero o camino y no hay agente
    // if ((c == 'S' || c == 'C') && a!= 'a') {
    //     if (visitasPorCasilla.at(posDelanteR(orientacion, posActual)) < MAX_VISITAS) {
    //         return 2;
    //     }
    // }

    // // 3. Valorar diagonal y visión extendida: más información => mejor exploración
    // if (c6 == 'S' || c6 == 'C') return 2;
    // if ((c5 == 'S' || c5 == 'C') return 1;
    // if ((c7 == 'S' || c7 == 'C') && ad != 'a') return 3;

    // // 4. Alternativas laterales si son sendero/camino y no hay agente
    // if ((i == 'S' || i == 'C') && ai != 'a') {
    //     if (visitasPorCasilla.at(posDelanteR((orientacion + 6) % 8, posActual)) < MAX_VISITAS) return 1;
    // }
    // if ((d == 'S' || d == 'C') && ad != 'a') {
    //     if (visitasPorCasilla.at(posDelanteR((orientacion + 2) % 8, posActual)) < MAX_VISITAS) return 3;
    // }

    // // 5. Si no hay nada interesante, retorna 0 para que el comportamiento principal decida qué hacer (evitar bucles, giros, etc.)
    // return 0;

}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{

	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	//1) Una primera fase de observación donde se actualizan los cambios de la última acción
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D'){
		tiene_zapatillas = true;
	}
	if(sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){ //noroeste
		cuadrantes[0]++;
	} else if(sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){ //noreste
		cuadrantes[1]++;
	} else if(sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){ //sureste
		cuadrantes[2]++;
	} else if(sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){ //suroeste
		cuadrantes[3]++;
	}

	pair<int, int> posActual = {sensores.posF, sensores.posC};
	//Para el case 0
	char terreno = 'C';  
    std::pair<int, int> frente = {0, 0}; 
	int MAX_GIROS_ALTERNOS = 2;

	//Guardamos la acción, orientación y visitas
	ultimaAccionPorCasilla[posActual] = last_action;
	ultimaOrientacionPorCasilla[posActual] = sensores.rumbo;
	//cout << visitasPorCasilla[posActual] << endl;

	//2) Una segunda fase donde se decide la siguiente acción a realizar
	if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	} else if(giro180 != 0){
		accion = TURN_L;
		giro180--;
	} else if(giro45dcha != 0){
		accion = TURN_SR;
		giro45dcha--;
	} else if(andar != 0){
		if(sensores.agentes[2]!='a'){
			accion = WALK;
		}else{
			accion = IDLE;
		}
		andar--;
	} else {
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
		char c6 = ViablePorAlturaR(sensores.superficie[6], sensores.cota[6] - sensores.cota[0], tiene_zapatillas);

		bool run = Run( sensores.agentes[2], sensores.agentes[6], sensores.superficie[2], c6);
		int pos = VeoCasillaIntersanteR1(i, c, d, tiene_zapatillas, sensores.agentes[2], sensores.agentes[1],sensores.agentes[3],sensores.superficie[5], sensores.superficie[6], sensores.superficie[7], sensores.rumbo, posActual, run);
		frente = posDelanteR(sensores.rumbo, posActual);
		terreno = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		bool casilla_frente_viable = ((terreno == 'C'||terreno=='S') && visitasPorCasilla[frente] < 3 && sensores.agentes[2] != 'a');

		if (!casilla_frente_viable &&
			visitasPorCasillaBucle[posActual] > 4 && (last_action == WALK||last_action == RUN))
		{
			buclePorCasilla[posActual]++;
			if (buclePorCasilla[posActual] >= 2) {
				if (giroAlterno < MAX_GIROS_ALTERNOS) {
					accion = TURN_L;
					giro180 = 1;
					giroAlterno++;
				} else {
					accion = TURN_SR;
					giroAlterno = 0;  
				}
				visitasPorCasillaBucle[posActual] -= 3;
				buclePorCasilla[posActual] = 0;
				last_action = accion;
				//visitasPorCasilla[posActual]++;
				visitasPorCasillaBucle[posActual]++;
				return accion;
			} else {
				if (giroAlterno < MAX_GIROS_ALTERNOS) {
					accion = TURN_L;
					giroAlterno++;
				} else {
					accion = TURN_SR;
					giro45dcha=2;
					giroAlterno = 0;  
				}
				last_action = accion;
				//visitasPorCasilla[posActual]++;
				visitasPorCasillaBucle[posActual]++;
				return accion;
			}
		}//else if(!casilla_frente_viable&&visitasPorCasillaBucle[posActual] > 15){
		// 	accion = IDLE;
		// 	last_action = accion;
		// 	giro45Izq = 0;
		// 	giro180 = 0;
		// 	giro45dcha = 0;
		// 	visitasPorCasillaBucle[posActual]=0;
		// 	return accion;
		// }
		


		switch (pos)
		{
		case 2:
			accion = WALK;
			switch (sensores.rumbo)
			{
			case sureste:
				if(sensores.agentes[3]=='a'){
					accion = IDLE;
				}
				break;
			case noreste:
				if(sensores.agentes[1]=='a'){
					accion = IDLE;
				}
				break;
			case suroeste:
				if(sensores.agentes[1]=='a'){
					accion = IDLE;
				}
				break;
			case noroeste:
				if(sensores.agentes[3]=='a'){
					accion = IDLE;
				}
				break;
			
			default:
				break;
			}
			// switch (sensores.rumbo){
			// 	case norte:
			// 	case este:
			// 	case sur:
			// 	case oeste:
			// 		accion = WALK;
			// 		if(sensores.agentes[1] == 'a' || sensores.agentes[3] == 'a' || sensores.agentes[5] == 'a' || sensores.agentes[6] == 'a' || sensores.agentes[7] == 'a'){
			// 			if(cuadrantes[0] >= 15 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[0] = 0;
			// 			} else if(cuadrantes[1] >= 15 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[1] = 0;
			// 			} else if(cuadrantes[2] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[2] = 0;
			// 			} else if(cuadrantes[3] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[3] = 0;
			// 			} else {
			// 				accion = TURN_L;
			// 			}
			// 		}
			// 		break;
			// 	case noreste:
			// 	case noroeste:
			// 	case sureste:
			// 	case suroeste:
			// 		accion = WALK;
			// 		if(sensores.agentes[1] == 'a' || sensores.agentes[3] == 'a' || sensores.agentes[4] == 'a' || sensores.agentes[5] == 'a' || sensores.agentes[6] == 'a' || sensores.agentes[7] == 'a' || sensores.agentes[8] == 'a'){
			// 			if(cuadrantes[0] >= 15 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[0] = 0;
			// 			} else if(cuadrantes[1] >= 15 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[1] = 0;
			// 			} else if(cuadrantes[2] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[2] = 0;
			// 			} else if(cuadrantes[3] >= 15 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
			// 				giro45dcha = 1;
			// 				accion = TURN_SR;
			// 				cuadrantes[3] = 0;
			// 			} else {
			// 				accion = TURN_L;
			// 			}
			// 		}
			// 		break;
			// }
			break;
		case 1:
			giro45Izq = 1;
			accion = TURN_L;
			break;
		case 3:
			accion = TURN_SR;
			break;
		case 0:  
			frente = posDelanteR(sensores.rumbo, posActual);

			// Verificamos si la casilla adelante es viable y si no se ha visitado muchas veces
			terreno = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
			if ((terreno == 'C'||terreno=='S') && visitasPorCasilla[frente] < 2 && sensores.agentes[2] != 'a') {
				accion = WALK;  // Camina si la casilla es viable y no la has visitado demasiado
			} else {
				if(cuadrantes[0] >= 20 && sensores.posF < lado_cuadrante && sensores.posC < lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[0] = 0;
				} else if(cuadrantes[1] >= 20 && sensores.posF < lado_cuadrante && sensores.posC >= lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[1] = 0;
				} else if(cuadrantes[2] >= 20 && sensores.posF >= lado_cuadrante && sensores.posC >= lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[2] = 0;
				} else if(cuadrantes[3] >= 20 && sensores.posF >= lado_cuadrante && sensores.posC < lado_cuadrante){
					giro45dcha = 1;
					accion = TURN_SR;
					cuadrantes[3] = 0;
				} else {
					accion = TURN_L;
					giro180 = 1;
				}
			}
			break;
		case 4:
			giro45Izq = 1;
			accion = TURN_L;
			andar = 1;
			break;
		case 5:
			accion = TURN_SR;
			andar = 1;
			break;
		case 6:
			accion = RUN;
			break;
		}
	}

	last_action = accion;
	visitasPorCasilla[posActual]++;
	visitasPorCasillaBucle[posActual]++;
	return accion;	

}


void AnularMatrizR(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const list<Action> &plan)
{
	AnularMatrizR(mapaConPlan);
	EstadoR cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
			case RUN:
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
				mapaConPlan[cst.f][cst.c] = 3;
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
				mapaConPlan[cst.f][cst.c] = 1;
				break;
			case TURN_SR:
				cst.brujula = (cst.brujula + 1) % 8;
				break;
			case TURN_L:
				cst.brujula = (cst.brujula + 6) % 8;
				break;
			}
		it++;
	}
}

void ComportamientoRescatador::PintaPlan(const list<Action> &plan, bool zap)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == WALK)
		{
			cout << "W ";
		}
		else if (*it == RUN)
		{
			cout << "R ";
		}
		else if (*it == TURN_SR)
		{
			cout << "r ";
		}
		else if (*it == TURN_L)
		{
			cout << "L ";
		}
		else if (*it == CALL_ON)
		{
			cout << "C ";
		}
		else if (*it == CALL_OFF)
		{
			cout << "c ";
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


EstadoR NextCasillaRescatador(const EstadoR &st){
	EstadoR siguiente = st;
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

bool ComportamientoRescatador::CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Action accion){
	
	bool check1 = false, check2 = false, check3=false;
	EstadoR intermedio = NextCasillaRescatador(st);
	EstadoR final = NextCasillaRescatador(intermedio);
	switch (accion)
	{
	case WALK:
		check1 = terreno[intermedio.f][intermedio.c] != 'P' and terreno[intermedio.f][intermedio.c] != 'M' and terreno[intermedio.f][intermedio.c] != 'B';
		check2 = (abs(altura[intermedio.f][intermedio.c] - altura[st.f][st.c]) <= 1) || (abs(altura[intermedio.f][intermedio.c] - altura[st.f][st.c]) <= 2)and st.zapatillas;
		
		return check1 and check2;
		
		break;
	case RUN:
		check1 = terreno[intermedio.f][intermedio.c] != 'P' and terreno[intermedio.f][intermedio.c] != 'M' and terreno[intermedio.f][intermedio.c] != 'B';
		check2 = terreno[final.f][final.c] != 'P' and terreno[final.f][final.c] != 'M' and terreno[final.f][final.c] != 'B';
		check3 = (abs(altura[final.f][final.c] - altura[st.f][st.c]) <= 1) || ((abs(altura[final.f][final.c] - altura[st.f][st.c]) <= 2)and st.zapatillas);
		
		return check1 and check2 and check3;
		
		break;
	default:
		break;
	}
}

EstadoR ComportamientoRescatador::applyR(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoR next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleRescatador(st,terreno,altura, WALK)){
				next = NextCasillaRescatador(st);
			}
			break;
		case RUN:
			if (CasillaAccesibleRescatador(st,terreno,altura, RUN)){
				EstadoR aux = NextCasillaRescatador(st);
				next = NextCasillaRescatador(aux);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula+1)%8;
			break;
		case TURN_L:
			next.brujula = (next.brujula+6)%8;
			break;
	}
	return next;
}

int ComportamientoRescatador::fixBateria_sig_accion_jugador(unsigned char celdaJugador, int difAltura, Action accion)
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

list<Action> ComportamientoRescatador::Dijkstra(const EstadoR &inicio, const EstadoR &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura) 
{
	priority_queue<NodoR> frontier;
	set<EstadoR> explorados;
	list<Action> plan;
	//std::cout << "Objetivo: " << final.f << "," << final.c << std::endl;

	NodoR current_node;
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
			EstadoR siguiente = NextCasillaRescatador(current_node.estado);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && CasillaAccesibleRescatador(current_node.estado, terreno, altura, WALK)){
				NodoR child_forward = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_forward.coste+=fixBateria_sig_accion_jugador(celda, difAltura, WALK);
				child_forward.estado = applyR(WALK, current_node.estado, terreno, altura);
				if (explorados.find(child_forward.estado) == explorados.end()){
					child_forward.secuencia.push_back(WALK);
					frontier.push(child_forward);
				}
			}
			//RUN
			EstadoR siguiente_ = NextCasillaRescatador(current_node.estado);
			siguiente = NextCasillaRescatador(siguiente_);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && 
						CasillaAccesibleRescatador(current_node.estado, terreno, altura, RUN)){
				NodoR child_run = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_run.coste+=fixBateria_sig_accion_jugador(celda, difAltura, RUN);
				child_run.estado = applyR(RUN, current_node.estado, terreno, altura);
				if (explorados.find(child_run.estado) == explorados.end()){
					child_run.secuencia.push_back(RUN);
					frontier.push(child_run);
				}
			}
			//TURN_L
			NodoR child_turnl = current_node;
			unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnl.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_L);
			child_turnl.estado = applyR(TURN_L, current_node.estado, terreno, altura);
			if (explorados.find(child_turnl.estado) == explorados.end()){
				child_turnl.secuencia.push_back(TURN_L);
				frontier.push(child_turnl);
			}
			//TURN_SR
			NodoR child_turnSr = current_node;
			celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnSr.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_SR);
			child_turnSr.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
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


Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;
	if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = Dijkstra(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio,plan);
		hayPlan = plan.size() != 0 ;
	}
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size()== 0){
		hayPlan = false;
	}
	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

bool ComportamientoRescatador::CasillaAccesibleRescatadorN4(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Action accion, Sensores sensores, bool sensores_validos){
	
	bool check1 = false, check2 = false, check3=false, check4=false;
	EstadoR intermedio = NextCasillaRescatador(st);
	EstadoR final = NextCasillaRescatador(intermedio);
	switch (accion)
	{
	case WALK:
		//Si la casilla es conocida
		if(intermedio.f>=0&&intermedio.f<tamanio_mapa&&intermedio.c>=0&&intermedio.c<tamanio_mapa){
			if(terreno[intermedio.f][intermedio.c]!='?'){
				check1 = terreno[intermedio.f][intermedio.c] != 'P' and terreno[intermedio.f][intermedio.c] != 'M' and terreno[intermedio.f][intermedio.c] != 'B';
				check2 = (abs(altura[intermedio.f][intermedio.c] - altura[st.f][st.c]) <= 1) || ((abs(altura[intermedio.f][intermedio.c] - altura[st.f][st.c]) <= 2)and st.zapatillas);
				if(sensores_validos){
					check3 = sensores.agentes[2]!='a';
				}else{
					check3=true;
				}

				return check1 and check2 and check3;
			}else{ //Si no dejamos que el rescatador pase por ahí, luego tenemos que manejar el caso que no sea accesible
				return true;
			}
		}else{
			return false;
		}
		
		
		break;
	case RUN:
		if(final.f>=0&&final.f<tamanio_mapa&&final.c>=0&&final.c<tamanio_mapa){
			//cout << terreno[final.f][final.c] << endl;
			if(terreno[intermedio.f][intermedio.c]!='?'&&terreno[final.f][final.c]!='?'){
				check1 = terreno[intermedio.f][intermedio.c] != 'P' and terreno[intermedio.f][intermedio.c] != 'M' and terreno[intermedio.f][intermedio.c] != 'B';
				check2 = terreno[final.f][final.c] != 'P' and terreno[final.f][final.c] != 'M' and terreno[final.f][final.c] != 'B';
				check3 = (abs(altura[final.f][final.c] - altura[st.f][st.c]) <= 1) || ((abs(altura[final.f][final.c] - altura[st.f][st.c]) <= 2)and st.zapatillas);
				if(sensores_validos){
					check4 = sensores.agentes[2]!='a' and sensores.agentes[6]!='a';
				}else{
					check4=true;
				} 

				return check1 and check2 and check3 and check4;
			}else{
				return true;
			}
		}else{
			return false;
		}
		
		
		break;
	default:
		break;
	}
}

EstadoR ComportamientoRescatador::applyRN4(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, Sensores sensores, bool sensores_validos){
	EstadoR next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleRescatadorN4(st,terreno,altura, WALK, sensores, sensores_validos)){
				next = NextCasillaRescatador(st);
			}
			break;
		case RUN:
			if (CasillaAccesibleRescatadorN4(st,terreno,altura, RUN, sensores, sensores_validos)){
				EstadoR aux = NextCasillaRescatador(st);
				next = NextCasillaRescatador(aux);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula+1)%8;
			break;
		case TURN_L:
			next.brujula = (next.brujula+6)%8;
			break;
	}
	return next;
}

list<Action> ComportamientoRescatador::DijkstraN4(const EstadoR &inicio, const EstadoR &final,
                                              const vector<vector<unsigned char>> &terreno,
                                              const vector<vector<unsigned char>> &altura, Sensores sensores) 
{
	priority_queue<NodoR> frontier;
	set<EstadoR> explorados;
	list<Action> plan;
	//std::cout << "Objetivo: " << final.f << "," << final.c << std::endl;

	NodoR current_node;
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
			if(current_node.estado==inicio){
				sensores_validos=true;
			}else{
				sensores_validos=false;
			}
			// Expandir hijos
			//WALK
			EstadoR siguiente = NextCasillaRescatador(current_node.estado);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && CasillaAccesibleRescatadorN4(current_node.estado, terreno, altura, WALK, sensores, sensores_validos)){
				NodoR child_forward = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_forward.coste+=fixBateria_sig_accion_jugador(celda, difAltura, WALK);
				child_forward.estado = applyRN4(WALK, current_node.estado, terreno, altura, sensores, sensores_validos);
				if (explorados.find(child_forward.estado) == explorados.end()){
					child_forward.secuencia.push_back(WALK);
					frontier.push(child_forward);
				}
			}
			//RUN
			EstadoR siguiente_ = NextCasillaRescatador(current_node.estado);
			siguiente = NextCasillaRescatador(siguiente_);
			if(((siguiente.f != current_node.estado.f) || (siguiente.c != current_node.estado.c)) && 
						CasillaAccesibleRescatadorN4(current_node.estado, terreno, altura, RUN, sensores, sensores_validos)){
				NodoR child_run = current_node;
				int difAltura = altura[siguiente.f][siguiente.c] - altura[current_node.estado.f][current_node.estado.c];
				
				unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
				child_run.coste+=fixBateria_sig_accion_jugador(celda, difAltura, RUN);
				child_run.estado = applyRN4(RUN, current_node.estado, terreno, altura, sensores, sensores_validos);
				if (explorados.find(child_run.estado) == explorados.end()){
					child_run.secuencia.push_back(RUN);
					frontier.push(child_run);
				}
			}
			//TURN_L
			NodoR child_turnl = current_node;
			unsigned char celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnl.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_L);
			child_turnl.estado = applyRN4(TURN_L, current_node.estado, terreno, altura, sensores, sensores_validos);
			if (explorados.find(child_turnl.estado) == explorados.end()){
				child_turnl.secuencia.push_back(TURN_L);
				frontier.push(child_turnl);
			}
			//TURN_SR
			NodoR child_turnSr = current_node;
			celda = terreno[current_node.estado.f][current_node.estado.c];
			child_turnSr.coste+=fixBateria_sig_accion_jugador(celda, 0, TURN_SR);
			child_turnSr.estado = applyRN4(TURN_SR, current_node.estado, terreno, altura, sensores, sensores_validos);
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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D'){
		tiene_zapatillas = true;
	}
	if(sensores.superficie[0] == 'X' and sensores.energia < 2000 && instante_simulacion-sensores.energia > 0){
		instante_simulacion--;
		last_action = IDLE;
		return IDLE;
	}
	Action accion = IDLE;
	if(sensores.gravedad and (last_action==WALK||last_action==RUN)){
		accion=CALL_ON;
		instantes_simulacion_auxiliar=1;
	}else if (sensores.gravedad and last_action!=CALL_OFF){
		accion=IDLE;
		instantes_simulacion_auxiliar++;
	}else if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = DijkstraN4(inicio, fin, mapaResultado, mapaCotas, sensores);
		VisualizaPlan(inicio,plan);
		hayPlan = plan.size() != 0 ;
	}
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();

		//Comprobamos que no haya obstaculos y sea casilla accesible
		if(accion==WALK||accion==RUN){
			EstadoR aux;
			aux.f = sensores.posF;
			aux.c = sensores.posC;
			aux.brujula = sensores.rumbo;
			aux.zapatillas = tiene_zapatillas;
			//cout << accion << endl;
			switch (accion)
			{
			case WALK:
				sensores_validos=true;
				if(!CasillaAccesibleRescatadorN4(aux, mapaResultado, mapaCotas, WALK,sensores, sensores_validos)){
					//cout << "hola";
					accion = IDLE;
					plan.clear();
				}
				break;
			case RUN:
				sensores_validos=true;
				if(!CasillaAccesibleRescatadorN4(aux, mapaResultado, mapaCotas, RUN, sensores, sensores_validos)){
					accion = IDLE;
					plan.clear();
				}
				break;
			
			default:
				break;
			}
		}

	}
	if (plan.size()== 0){
		hayPlan = false;
	}

	if(instantes_simulacion_auxiliar>=200){
		accion=CALL_OFF;
		instantes_simulacion_auxiliar=0;
	}

	instante_simulacion--;
	last_action = accion;
	return accion;
}

void ComportamientoRescatador::SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores){

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
