// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "ListaUsuarios.h"
#include "ListaPartidas.h"

//TIPOS

const int MAX_PAR_US = 5; // Máximo de partidas en curso que un usuario puede tener

typedef struct {
	std::string idpart; // Identificador de la partida (en curso)
	tPartida enlace; // Puntero que apunta a la direccion de la partida en curso
}tParIdEn;  //Definimos el tipo tParIdEn

typedef tParIdEn tArrayPartidasEnCurso[MAX_PAR_US];  //Definimos el tipo tArrayPartidasEnCurso

typedef struct {
	tArrayPartidasEnCurso elementos; // elementos de la lista de partidas en curso
	int contador; //contador de elementos de dicha lista
}tListaAccesoPartidas;  //Definimos el tipo tListaAccesoPartidas

typedef struct {
	tListaPartidas partidas; //única copia de partidas de la aplicación
	tListaUsuarios usuarios; // lista de usuarios del sistema
	tListaAccesoPartidas listaAcceso;
	int posusu; //índice del usuario que inicia sesión en la lista de usuarios
	tPartida indpart; //partida seleccionada por el usuario de la sesión	
}tGestor;  //Definimos el tipo tGestor

//FUNCIONES

bool buscar(tListaAccesoPartidas const& listaAcceso, std::string const& id, int &/*in/out*/ pos);

bool arrancar(tGestor &gestor); //Inicializa el gestor, cargando la información del sistema (abre los archivos, carga las listas, etc.)

void generarAccesos(tGestor &gestor); //Recorre la lista de partidas, y para las que están en curso, añaden el id y el indice a la lista de partidas en curso de cada uno de los adversarios

void apagar(tGestor &gestor); //Apaga el gestor, abriendo los ficheros para guardar las listas de usuarios y de partidas del sistema

void redimensionarLista(tGestor &gestor); //Función que permite modificar la dimensión de la lista de partidas

void redimensionarUsuarios(tGestor &gestor); //Función que permite modificar la dimensión de la lista de usuarios

void detectarPartidasTerminadas(tGestor &gestor); //Detectamos qué partidas llevan 30 días sin actividad

void mensajePartidaDetectada(tUsuario ganador, tUsuario perdedor); //Actualiza los avisos de los usuarios que han sido afectados por la finalización de una partida por inactividad

//FUNCIONES DEL MENÚ REGISTRO

bool iniciarSesion(tGestor &gestor, const std::string &idUsu, const std::string &clave); //Función que lleva a cabo el inicio de sesión de un usuario dado su nombre y su clave

bool crearCuenta(tGestor &gestor, const std::string &idUsu, const std::string &clave);  //Función que crea una cuenta nueva dado un nombre de usuario y una contraseña

bool tieneAvisos(const tGestor &gestor, std::string &aviso);  //Función que determina si el usuario tiene avisos

void limpiarAvisos(tGestor &gestor); //Función que limpia los avisos del usuario borrando todos

std::string resumenActividad(const tGestor &gestor);  //Función que devuelve un resumen de la actividad del usuario (racha, ganadas, perdidas,...)

void mostrarActividad(const tGestor &gestor); //Función que se encarga de mostrar por pantalla la actividad del usuario

//FUNCIONES DEL MENÚ USUARIO

int partidasUsuario(const tGestor &gestor); //Función que devuelve el número de partidas en curso que tiene el usuario

std::string cabecera(const tGestor &gestor); //Función que muestra la información de una partida del usuario

void ordenar_Fecha(tGestor &gestor); //Función que ordena las partidas de un usuario por fecha

void ordenar_Turno(tGestor &gestor); //Función que ordena las partidas de un usuario por turno

bool nuevaPartida(tGestor &gestor);  //Función que crea una nueva partida

void apuntaPartida(tGestor &gestor, int posParEnCurso);  //Función que actualiza el campo gestor.indpart indicando cuál es la partida seleccionada por el usuario

//FUNCIONES DEL MENÚ PARTIDA

void mostrarPartida(const tGestor &gestor);  //Función que muestra una partida 

bool esSuTurno(const tGestor &gestor);  //Función que determina si es el turno del usuario seleccionado en una partida

void jugarPartida(tGestor &gestor, int col); //Función que dado un movimiento, aplica la jugada y actualiza los adversarios si es necesario

void eliminarPartidaTerminada(tGestor &gestor, const std::string &id);  //Función que elimina una partida terminada

void abandonarPartida(tGestor &gestor);  //Función que aplica la decisión del usuario de abandonar una partida