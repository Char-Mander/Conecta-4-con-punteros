// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "ListaUsuarios.h"
#include "ListaPartidas.h"

//TIPOS

const int MAX_PAR_US = 5; // M�ximo de partidas en curso que un usuario puede tener

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
	tListaPartidas partidas; //�nica copia de partidas de la aplicaci�n
	tListaUsuarios usuarios; // lista de usuarios del sistema
	tListaAccesoPartidas listaAcceso;
	int posusu; //�ndice del usuario que inicia sesi�n en la lista de usuarios
	tPartida indpart; //partida seleccionada por el usuario de la sesi�n	
}tGestor;  //Definimos el tipo tGestor

//FUNCIONES

bool buscar(tListaAccesoPartidas const& listaAcceso, std::string const& id, int &/*in/out*/ pos);

bool arrancar(tGestor &gestor); //Inicializa el gestor, cargando la informaci�n del sistema (abre los archivos, carga las listas, etc.)

void generarAccesos(tGestor &gestor); //Recorre la lista de partidas, y para las que est�n en curso, a�aden el id y el indice a la lista de partidas en curso de cada uno de los adversarios

void apagar(tGestor &gestor); //Apaga el gestor, abriendo los ficheros para guardar las listas de usuarios y de partidas del sistema

void redimensionarLista(tGestor &gestor); //Funci�n que permite modificar la dimensi�n de la lista de partidas

void redimensionarUsuarios(tGestor &gestor); //Funci�n que permite modificar la dimensi�n de la lista de usuarios

void detectarPartidasTerminadas(tGestor &gestor); //Detectamos qu� partidas llevan 30 d�as sin actividad

void mensajePartidaDetectada(tUsuario ganador, tUsuario perdedor); //Actualiza los avisos de los usuarios que han sido afectados por la finalizaci�n de una partida por inactividad

//FUNCIONES DEL MEN� REGISTRO

bool iniciarSesion(tGestor &gestor, const std::string &idUsu, const std::string &clave); //Funci�n que lleva a cabo el inicio de sesi�n de un usuario dado su nombre y su clave

bool crearCuenta(tGestor &gestor, const std::string &idUsu, const std::string &clave);  //Funci�n que crea una cuenta nueva dado un nombre de usuario y una contrase�a

bool tieneAvisos(const tGestor &gestor, std::string &aviso);  //Funci�n que determina si el usuario tiene avisos

void limpiarAvisos(tGestor &gestor); //Funci�n que limpia los avisos del usuario borrando todos

std::string resumenActividad(const tGestor &gestor);  //Funci�n que devuelve un resumen de la actividad del usuario (racha, ganadas, perdidas,...)

void mostrarActividad(const tGestor &gestor); //Funci�n que se encarga de mostrar por pantalla la actividad del usuario

//FUNCIONES DEL MEN� USUARIO

int partidasUsuario(const tGestor &gestor); //Funci�n que devuelve el n�mero de partidas en curso que tiene el usuario

std::string cabecera(const tGestor &gestor); //Funci�n que muestra la informaci�n de una partida del usuario

void ordenar_Fecha(tGestor &gestor); //Funci�n que ordena las partidas de un usuario por fecha

void ordenar_Turno(tGestor &gestor); //Funci�n que ordena las partidas de un usuario por turno

bool nuevaPartida(tGestor &gestor);  //Funci�n que crea una nueva partida

void apuntaPartida(tGestor &gestor, int posParEnCurso);  //Funci�n que actualiza el campo gestor.indpart indicando cu�l es la partida seleccionada por el usuario

//FUNCIONES DEL MEN� PARTIDA

void mostrarPartida(const tGestor &gestor);  //Funci�n que muestra una partida 

bool esSuTurno(const tGestor &gestor);  //Funci�n que determina si es el turno del usuario seleccionado en una partida

void jugarPartida(tGestor &gestor, int col); //Funci�n que dado un movimiento, aplica la jugada y actualiza los adversarios si es necesario

void eliminarPartidaTerminada(tGestor &gestor, const std::string &id);  //Funci�n que elimina una partida terminada

void abandonarPartida(tGestor &gestor);  //Funci�n que aplica la decisi�n del usuario de abandonar una partida