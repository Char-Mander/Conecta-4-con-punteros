// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "Usuario.h"
#include "checkML.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

bool cargar(tUsuario &usuario, ifstream &archivo) { //Dado un flujo de archivo, lee los datos de un usuario
	const string centinela = "_X_X_X_";  //Declaramos el centinela para facilitar su búsqueda
	int aux;  //Empleamos una variable auxiliar para convertir el entero del .txt al tipo tNivel
	ostringstream todoslosavisos;  //Flujo que emplearemos para cargar los avisos
	string avisosaux;  //String en el que guardaremos los avisos
	char chaux;  //Char auxiliar para saltar de línea

	archivo >> usuario->nombre;  //Cargamos los datos del usuario del .txt a tUsuario usuario
	archivo >> usuario->pas; 
	archivo >> aux;
	usuario->nivel = tNivel(aux);
	archivo >> usuario->racha;
	archivo >> usuario->esperando;
	archivo >> usuario->ganadas;
	archivo >> usuario->perdidas;
	archivo >> usuario->empatadas;
	archivo >> usuario->ultConexion;
	archivo.get(chaux);
	
	getline(archivo, avisosaux);
	if (avisosaux != centinela) {  //En caso de que posea avisos procedemos a su carga
		todoslosavisos << avisosaux;
		getline(archivo, avisosaux);
		while (avisosaux != centinela) {
			todoslosavisos << '\n' << avisosaux;
			getline(archivo, avisosaux);
		}
		todoslosavisos << '\n';
		usuario->avisos = todoslosavisos.str();
	}

	return !archivo.fail();
}

void guardar(const tUsuario &usuario, ofstream &archivo) {  //Dado un flujo de archivo, escribe en él los datos del usuario
	const string centinela = "_X_X_X_";  //Declaramos el centinela para guardarlo con mayor facilidad

	archivo << usuario->nombre << '\n';  // Guardamos los datos del usuario en el .txt
	archivo << usuario->pas << '\n';
	archivo << usuario->nivel << " " << usuario->racha << " " << usuario->esperando << '\n';
	archivo << usuario->ganadas << " " << usuario->perdidas << " " << usuario->empatadas << '\n';
	archivo << usuario->ultConexion << '\n';
	archivo << usuario->avisos;


	archivo << centinela << '\n'; //Se incluye el centinela al guardar el usuario

}

void iniciar(tUsuario &usuario, const string &id, const string &pas) {  //Recibe un nombre  de usuario y una contraseña, e inicia todos los campos del usuario
	ostringstream avisosVacíos;  //Flujo que emplearemos para los avisos del usuario

	usuario->nombre = id;  //Iniciamos todos los datos del usuario
	usuario->pas = pas;
	usuario->nivel = Principiante;
	usuario->racha = 0;
	usuario->esperando = false;
	usuario->ganadas = 0;
	usuario->perdidas = 0;
	usuario->empatadas = 0;
	usuario->ultConexion = fechaActual();
	avisosVacíos << '\n';
	usuario->avisos = avisosVacíos.str(); // Cuando no hay ningún aviso, simplemente se pone un salto de línea	
}

bool iniciarSesion(tUsuario &usuario, const string &clave) {  // Recibe una contraseña y un usuario, y se comprueba si la contraseña es correcta o no
	bool correcta = false;

	if (usuario->pas == clave) {  //Determinamos si la clave introducida es correcta o no
		correcta = true;
		usuario->ultConexion = fechaActual();
	}

	return correcta;
}

void actualizarAvisos(tUsuario &usu, const string &aviso) {  // Actualiza el campo de avisos, añadiendo al final una nueva línea con el nuevo aviso
	ostringstream todoslosavisos, aux;  //Flujos que empleamos para actualizar los avisos
	aux << '\n';
	if (usu->avisos != aux.str()) {  //En caso de que tenga avisos ya:
		todoslosavisos << usu->avisos << aviso << '\n';
		usu->avisos = todoslosavisos.str();
	}
	else {  //En caso de que no tenga avisos todavía:
		todoslosavisos << aviso << '\n';
		usu->avisos = todoslosavisos.str();
	}
}

void limpiarAvisos(tUsuario &usu) {  // Actualiza el campo avisos, dejando el string vacío
	ostringstream avisosVacíos;
	
	avisosVacíos << '\n';
	usu->avisos = avisosVacíos.str();
}

void aplicarFinPartida(tUsuario &usuario, tResultado resultado) {
	ostringstream aviso;  //Flujo para facilitar el guardado de avisos de los usuarios

	if (resultado == Gana) { //En caso de que el usuario gane:
		usuario->ganadas++;  //Sumamos '1' a las gandas
		if (usuario->racha >= 0) { //En caso de que la racha sea mayor o igual que '0', sumaremos uno a la racha; en caso contrario la actualizaremos a '1'
			usuario->racha++;
			if (usuario->racha == NRN || (usuario->racha%NRN == 0 && usuario->racha > NRN)) { //Comprobamos el número de la racha para saber si este sube de nivel; para ello comparamos si es igual a la constante "NRN".
				switch (usuario->nivel) {                                                   // Con el módulo de "NRN", podremos determinar si es su segunda (o más de dos) racha de partidas ganadas consecutivamente.
				case Principiante: usuario->nivel = Medio; aviso << "Has subido al nivel medio en la fecha " << stringFecha(fechaActual(), false); break; //Informamos al usuario de su subida de nivel
				case Medio:	usuario->nivel = Experto; aviso << "Has subido al nivel experto en la fecha " << stringFecha(fechaActual(), false); break;
				case Experto: break;
				}
				actualizarAvisos(usuario, aviso.str());  //Actualizamos los avisos de subida de nivel
			}
		}
		else usuario->racha = 1;
	}
	else if (resultado == Pierde) {  //En caso de que el usuario pierda:
		usuario->perdidas++;  //Sumamos una a perdidas
		if (usuario->racha <= 0) { //Si la racha es menor o igual que '0', restamos '1' a la racha; en caso de que sea mayor de 0 la actualizamos a '-1'
			usuario->racha--;
			if (usuario->racha == (-NRN) || (usuario->racha % -NRN == 0 && usuario->racha < -NRN)){ //Comprobamos el numero de la racha para saber si este baja de nivel, para ello comparamos si es igual a la constante "NRN".
				switch (usuario->nivel) {                                                          // Con el módulo de "NRN", podremos determinar si es su segunda (o más de dos) racha de partidas perdidas consecutivamente.
				case Principiante: break;
				case Medio: usuario->nivel = Principiante; aviso << "Has bajado al nivel medio en la fecha " << stringFecha(fechaActual(), false); break; //Informamos al usuario de su bajada de nivel
				case Experto: usuario->nivel = Medio; aviso << "Has bajado al nivel medio en la fecha " << stringFecha(fechaActual(), false); break;
				}
				actualizarAvisos(usuario, aviso.str());
			}
		}
		else usuario->racha = -1;
	}
	else {  //En caso de que el usuario empate, actualizamos su racha a '0' y sumamos '1' a empatadas
		usuario->racha = 0;
		usuario->empatadas++;
	}
}