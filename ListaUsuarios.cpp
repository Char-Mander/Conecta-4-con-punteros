// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "ListaUsuarios.h"
#include "checkML.h"
#include <string>
#include <fstream>

using namespace std;

bool cargar(tListaUsuarios &usuarios, ifstream &archivo){  //Carga la lista de usuarios desde el flujo archivo
bool ok = true;
	int pos = 0;
	int indice = 0;	
	while (ok && pos < usuarios.contador) {  //Procedemos a guardar cada usuario del archivo, comprobando siempre que la carga se haya realizado correctamente
		usuarios.listausuarios[pos] = new tUsu;
		ok = cargar(usuarios.listausuarios[pos], archivo); 												
		indice = pos;
		ok = insertar(usuarios, usuarios.listausuarios[pos], indice);
		pos++;
	}

	return ok;
}

void apagar(const tListaUsuarios &usuarios, ofstream &archivo) {  // Guarda la lista de uusarios en el flujo archivo

	archivo << usuarios.contador << '\n';  //Guardamos el contador de usuarios

	for (int pos = 0; pos< usuarios.contador; pos++) {  //Recorremos la lista de usuarios guardando cada uno		
		guardar(usuarios.listausuarios[pos], archivo);
		delete usuarios.listausuarios[pos];
	}
	delete [] usuarios.listausuarios;
}

bool buscarUsuarioEsperando(const tListaUsuarios &usuarios, tNivel nivel, int &pos) {  // Devuelve la posición del primer usuario del nivel dado que esté esperando para iniciar una partida (si lo hay)
	bool encontrado = false;
	int contador = 0;

	do { //Recorremos la lista de usuarios, buscando el primero que tenga el estado esperando, y devolvemos su posición en la lista por 'int pos'
		if (usuarios.listausuarios[contador]->nombre!= usuarios.listausuarios[pos]->nombre &&  usuarios.listausuarios[contador]->nivel == nivel && usuarios.listausuarios[contador]->esperando) {
			encontrado = true;
			pos = contador;
		}
		else contador++;
	} while (!encontrado && contador<usuarios.contador);

	return encontrado;
}

bool insertar(tListaUsuarios &usuarios, const tUsuario &usuario, int &pos) {  // Añade un usuario en la posición de la lista que le corresponde, ordenadamente (siempre que no haya uno con el mismo nombre)
	tUsuario tmp;
	int ind = pos;
	bool sePuede = true;

	if (!buscar(usuarios, usuarios.listausuarios[pos]->nombre, ind)) { //Comprobamos que no haya ningún usuario con el mismo nombre

		usuarios.listausuarios[pos] = usuario; 

		for (int posicion = pos; posicion > ind; posicion--) {  //Actualizamos la lista de usuarios, situando al nuevo usuario en la posición que le corresponde
			tmp = usuarios.listausuarios[posicion - 1];
			usuarios.listausuarios[posicion - 1] = usuarios.listausuarios[posicion];
			usuarios.listausuarios[posicion] = tmp;
		}

		pos = ind;
	}
	else sePuede = false;

	return sePuede;
}

bool buscar(const tListaUsuarios &usuarios, const string &idUser, int &pos) {   // Dado un identificador de usuario y una lista, comprueba si existe el id en la lista (si existe devuelve su posición)
	int ini = 0;																//si no está, devuelve la posición que le correspondería	
	int fin = pos-1;
	int mitad;
	bool encontrado = false;

	while ((ini <= fin) && !encontrado) {  //Realizamos una búsqueda binaria para encontrar al usuario solicitado
		mitad = (ini + fin) / 2; // División entera
		if (idUser == usuarios.listausuarios[mitad]->nombre) {
			encontrado = true;
			pos = mitad;
		}
		else if (idUser < usuarios.listausuarios[mitad]->nombre) {
			fin = mitad-1;
			pos = fin;
		}
		else {
			ini = mitad + 1;
			pos = ini;
		}
	} 

	if (!encontrado && ini>fin) {
		pos = ini;
	}

	return encontrado;
}