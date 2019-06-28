// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#ifndef ListaPartidas_h
#define ListaPartidas_h
#include "Partida.h"

//CONSTANTES Y TIPOS

typedef struct {
	int dimensionPartidas; //Tamaño del array (de datos dinámicos) de la lista de partidas
	int contador;		//Contador de los elementos de la lista
	tPartida *partida; //Elementos de la lista
}tListaPartidas;  //Definimos el tipo tListaPartidas

//FUNCIONES

void apagar(const tListaPartidas &partidas, std::ofstream &archivo); // Guarda la lista de partidas en el flujo archivo.

int numeroPartidasTerminadas(const tListaPartidas &partidas); //Devuelve el número de partidas terminadas.

bool cargar(tListaPartidas &partidas, std::ifstream &archivo); //Carga la lista de partidas desde el flujo archivo.

bool insertar(tListaPartidas &partidas, const tPartida &partida); // Dada una partida, si hay espacio en la lista, la añade al final de esta.

#endif ListaPartidas_h