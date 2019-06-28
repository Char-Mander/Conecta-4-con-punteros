// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12
#include "ListaPartidas.h"

//CONSTANTES Y TIPOS

int const MAX_IDS = 20;  //IDS máximos que puede tener una lista de estadísticas.

typedef struct{ //Estructurado del tipo tEstadistica 
	std::string id;				//ID del usuario de la sesión
	std::string adversario;		//ID del adversario (del usuario de la sesión) en cada partida
	int ganadas;	//Partidas ganadas del usuario de la sesión		
	int perdidas;	//Partidas perdidas del usuario de la sesión		
	int empatadas;  //Partidas empatadas del usuario de la sesión		
}tEstadistica;

typedef tEstadistica *tEstadisticas; //Definimos el tipo (puntero) tEstadísticas

typedef struct{ //Estructurado del tipo tListaEstadísticas
	tEstadisticas *listaEstadisticas; // Lista de las estadísticas (solo guardará las partidas ya terminadas)
	int contador;  //Cantidad de partidas que hay en la lista de estadísticas
	int tamañoEstadisticas; //Tamaño del array (de datos dinámicos) de la lista de las estadísticas
}tListaEstadisticas; 

typedef struct { //Estructurado del tipo tListaIdExistentes
	std::string idExistente[MAX_IDS]; //Array (de datos dinámicos) que guarda los id de las partidas en curso que ya han aparecido previamente
	int contador;		//Cantidad de elementos del array de ID
} tListaIdExistentes;

//FUNCIONES

void cargarEstadisticas(tListaEstadisticas &lista, std::string usuario); // Función que carga la lista de todas las estadísticas.

bool existeElAdversario(const tListaEstadisticas &lista, const std::string adversario, int &pos); //Función que comprueba si ya ha guardado previamente a dicho adversario en la lista de estadísticas.
																								  //Si está en el array, devolverá la posición en la que esté
 
void iniciarEstadistica(tEstadisticas estadistica, std::string adversario); //Función que crea un nuevo adversario en la lista

void apagarEstadisticas(tListaEstadisticas &lista); //Elimina el array (de datos dinámicos) que guarda la lista de estadísticas

void actualizarEstadisticas(tListaEstadisticas &lista, std::string usuario, const tListaPartidas &partidas, tListaIdExistentes &listaEx);//Función que actualiza la lista de estadísticas

bool buscarPartidaExistente(const tListaIdExistentes &lista, const std::string id); //Función que comprueba el id de la partida está en la lista de partidas existentes.

std::ostringstream mostrarEstadisticas(const tListaEstadisticas &lista); //Función que muestra la lista de las estadísticas del usuario de la sesión.

void actualizarFlujoEstadisticas(const tListaEstadisticas &lista, std::ostringstream &flujo, int &pos); //Función que actualiza el flujo con los datos de las estadísticas

void redimensionarEstadisticas(tListaEstadisticas &lista); //Función que sirve para redimensionar el array de la lista de estadísticas.