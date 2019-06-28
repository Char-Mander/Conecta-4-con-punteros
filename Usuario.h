// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "fecha.h"

//CONSTANTES Y TIPOS

const int NRN = 5;//Número de partidas ganadas/perdidas (racha) necesarias para subir/bajar de nivel 

typedef enum {Principiante, Medio, Experto} tNivel; //tipo enumerado tNivel

typedef enum {Gana, Pierde, Empata} tResultado; //tipo enumerado tResultado

typedef struct {
	std::string nombre; //Nombre de un usuario, ÚNICO en el gestor
	std::string pas; // Contraseña del usuario
	tNivel nivel; //Nivel de juego 
	int racha; //Última racha del usuario
	bool esperando; //Si está esperando o no un adversario para empezar una nueva partida
	int ganadas, perdidas, empatadas;//Número de partidas ganadas, perdidas, o empatadas
	tFecha ultConexion; //Fecha de la última conexión
	std::string avisos; //Avisos que le envía el gestor	
}tUsu;

typedef tUsu *tUsuario; //Definimos el tipo (puntero) tUsuario


//FUNCIONES

bool cargar(tUsuario &usuario, std::ifstream &archivo); //Dado un flujo de archivo, lee los datos de un usuario

void guardar(const tUsuario &usuario, std::ofstream &archivo); //Dado un flujo de archivo, escribe en él los datos del usuario

void iniciar(tUsuario &usuario, const std::string &id, const std::string &pas); //Recibe un nombre  de usuario y una contraseña, e inicia todos los campos del usuario

bool iniciarSesion(tUsuario &usuario, const std::string &clave); // Recibe una contraseña y un usuario, y se comprueba si la contraseña es correcta o no

void actualizarAvisos(tUsuario &usu, const std::string &aviso); // Actualiza el campo de avisos, añadiendo al final una nueva línea con el nuevo aviso

void limpiarAvisos(tUsuario &usu); // Actualiza el campo avisos, dejando el string vacío

void aplicarFinPartida(tUsuario &usuario, tResultado resultado); // Modifica los cambios afectados al final de una partida en curso con el resultado dado