// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "Usuario.h"

//CONSTANTES Y TIPOS

typedef struct {
	tUsuario *listausuarios; //Elementos de la lista
	int contador; //Contador de usuarios de la lista
	int dimensionUsuarios; //Dimensi�n del array (de datos din�micos) de la lista de usuarios
}tListaUsuarios;  //Declaramos el tipo tListaUsuarios

//FUNCIONES

bool cargar(tListaUsuarios &usuarios, std::ifstream &archivo);	//Carga la lista de usuarios desde el flujo archivo

void apagar(const tListaUsuarios &usuarios, std::ofstream &archivo); // Guarda la lista de usuarios en el flujo archivo

bool buscarUsuarioEsperando(const tListaUsuarios &usuarios, tNivel nivel, int &pos); // Devuelve la posici�n del primer usuario del nivel dado que est� esperando para iniciar una partida (si lo hay)

bool insertar(tListaUsuarios &usuarios, const tUsuario &usuario, int&pos); // A�ade un usuario en la posici�n de la lista que le corresponde, ordenadamente (siempre que no haya uno con el mismo nombre)

bool buscar(const tListaUsuarios &usuarios, const std::string &idUser, int &pos); // Dado un identificador de usuario y una lista, comprueba si existe el id en la lista (si existe devuelve su posici�n)
																				//si no est�, devuelve la posici�n que le corresponder�a
