// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12
#include "ListaPartidas.h"

//CONSTANTES Y TIPOS

int const MAX_IDS = 20;  //IDS m�ximos que puede tener una lista de estad�sticas.

typedef struct{ //Estructurado del tipo tEstadistica 
	std::string id;				//ID del usuario de la sesi�n
	std::string adversario;		//ID del adversario (del usuario de la sesi�n) en cada partida
	int ganadas;	//Partidas ganadas del usuario de la sesi�n		
	int perdidas;	//Partidas perdidas del usuario de la sesi�n		
	int empatadas;  //Partidas empatadas del usuario de la sesi�n		
}tEstadistica;

typedef tEstadistica *tEstadisticas; //Definimos el tipo (puntero) tEstad�sticas

typedef struct{ //Estructurado del tipo tListaEstad�sticas
	tEstadisticas *listaEstadisticas; // Lista de las estad�sticas (solo guardar� las partidas ya terminadas)
	int contador;  //Cantidad de partidas que hay en la lista de estad�sticas
	int tama�oEstadisticas; //Tama�o del array (de datos din�micos) de la lista de las estad�sticas
}tListaEstadisticas; 

typedef struct { //Estructurado del tipo tListaIdExistentes
	std::string idExistente[MAX_IDS]; //Array (de datos din�micos) que guarda los id de las partidas en curso que ya han aparecido previamente
	int contador;		//Cantidad de elementos del array de ID
} tListaIdExistentes;

//FUNCIONES

void cargarEstadisticas(tListaEstadisticas &lista, std::string usuario); // Funci�n que carga la lista de todas las estad�sticas.

bool existeElAdversario(const tListaEstadisticas &lista, const std::string adversario, int &pos); //Funci�n que comprueba si ya ha guardado previamente a dicho adversario en la lista de estad�sticas.
																								  //Si est� en el array, devolver� la posici�n en la que est�
 
void iniciarEstadistica(tEstadisticas estadistica, std::string adversario); //Funci�n que crea un nuevo adversario en la lista

void apagarEstadisticas(tListaEstadisticas &lista); //Elimina el array (de datos din�micos) que guarda la lista de estad�sticas

void actualizarEstadisticas(tListaEstadisticas &lista, std::string usuario, const tListaPartidas &partidas, tListaIdExistentes &listaEx);//Funci�n que actualiza la lista de estad�sticas

bool buscarPartidaExistente(const tListaIdExistentes &lista, const std::string id); //Funci�n que comprueba el id de la partida est� en la lista de partidas existentes.

std::ostringstream mostrarEstadisticas(const tListaEstadisticas &lista); //Funci�n que muestra la lista de las estad�sticas del usuario de la sesi�n.

void actualizarFlujoEstadisticas(const tListaEstadisticas &lista, std::ostringstream &flujo, int &pos); //Funci�n que actualiza el flujo con los datos de las estad�sticas

void redimensionarEstadisticas(tListaEstadisticas &lista); //Funci�n que sirve para redimensionar el array de la lista de estad�sticas.