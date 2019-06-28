// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "fecha.h"

//CONSTANTES Y TIPOS

const int NRN = 5;//N�mero de partidas ganadas/perdidas (racha) necesarias para subir/bajar de nivel 

typedef enum {Principiante, Medio, Experto} tNivel; //tipo enumerado tNivel

typedef enum {Gana, Pierde, Empata} tResultado; //tipo enumerado tResultado

typedef struct {
	std::string nombre; //Nombre de un usuario, �NICO en el gestor
	std::string pas; // Contrase�a del usuario
	tNivel nivel; //Nivel de juego 
	int racha; //�ltima racha del usuario
	bool esperando; //Si est� esperando o no un adversario para empezar una nueva partida
	int ganadas, perdidas, empatadas;//N�mero de partidas ganadas, perdidas, o empatadas
	tFecha ultConexion; //Fecha de la �ltima conexi�n
	std::string avisos; //Avisos que le env�a el gestor	
}tUsu;

typedef tUsu *tUsuario; //Definimos el tipo (puntero) tUsuario


//FUNCIONES

bool cargar(tUsuario &usuario, std::ifstream &archivo); //Dado un flujo de archivo, lee los datos de un usuario

void guardar(const tUsuario &usuario, std::ofstream &archivo); //Dado un flujo de archivo, escribe en �l los datos del usuario

void iniciar(tUsuario &usuario, const std::string &id, const std::string &pas); //Recibe un nombre  de usuario y una contrase�a, e inicia todos los campos del usuario

bool iniciarSesion(tUsuario &usuario, const std::string &clave); // Recibe una contrase�a y un usuario, y se comprueba si la contrase�a es correcta o no

void actualizarAvisos(tUsuario &usu, const std::string &aviso); // Actualiza el campo de avisos, a�adiendo al final una nueva l�nea con el nuevo aviso

void limpiarAvisos(tUsuario &usu); // Actualiza el campo avisos, dejando el string vac�o

void aplicarFinPartida(tUsuario &usuario, tResultado resultado); // Modifica los cambios afectados al final de una partida en curso con el resultado dado