// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "Partida.h"
#include "checkML.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

void nueva(tPartida &partida, const string &jugador1, const string &jugador2) {  //recibe los identificadores y devuelve una partida con todos sus datos rellenos
	ostringstream idcompleto;  //Flujo para facilitar la creación del id de la partida
	                                  //Iniciamos todos los datos de la partida
	partida->fechaIni = fechaActual();  
	idcompleto << partida->fechaIni << "_" << jugador1 << "_" << jugador2;
	partida->id = idcompleto.str();
	partida->jugador1 = jugador1;
	partida->jugador2 = jugador2;
	partida->fechaActu = partida->fechaIni;
	partida->estadoPartida = EnCurso;
	iniciar(partida->estadoJuego);
}

void guardar(const tPartida &partida, ofstream &archivo) {  //Dado un flujo de archivo de salida, lee los datos que corresponden a una partida		
	archivo << partida->id << '\n';  //Guardamos todos los datos de la partida en el archivo
	archivo << partida->jugador1 << '\n';
	archivo << partida->jugador2 << '\n';
	archivo << partida->fechaIni << " " << partida->fechaActu << '\n';
	archivo << partida->estadoPartida << '\n';
	guardar(partida->estadoJuego, archivo);			
} 

bool cargar(tPartida &partida, ifstream &archivo) {  //Dado un flujo de archivo de entrada, lee los datos que corresponden a una partida
	int aux;	
	bool ok;
	                         // Cargamos todos los datos de la partida
	archivo >> partida->id;
	archivo >> partida->jugador1;
	archivo >> partida->jugador2;
	archivo >> partida->fechaIni;
	archivo >> partida->fechaActu;
	archivo >> aux;
	partida->estadoPartida = tEstadoPartida(aux);
	if (archivo.fail()) { //Comprobamos que la carga se ha realizado correctamente
		ok = false;
	}
	else ok = cargar(partida->estadoJuego, archivo);  //Cargamos y comprobamos que se ha cargado correctamente el estado del juego
	
	return ok;
} 

void abandonar(tPartida &partida) {  //Modifica el estado  de la partida a Terminada y la fecha de la última actualización
	partida->fechaActu = fechaActual(); //Actualizamos fecha
	partida->estadoPartida = Terminada; //Actualizamos estado	
}

bool aplicarJugada(tPartida &partida, int col) {  //Aplica la jugada col al juego
	bool sePuede;	

	sePuede = aplicarJugada(partida->estadoJuego, col); //Actualizamos el valor de bool sePuede a si ha sido posible aplicar la jugada
	
	return sePuede;
}