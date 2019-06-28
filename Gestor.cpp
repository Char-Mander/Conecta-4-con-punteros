// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "Gestor.h"
#include "checkML.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

bool arrancar(tGestor &gestor) { //Inicializa el gestor, cargando la información del sistema (abre los archivos, carga las listas, etc.)

	ifstream archivopart, archivousu; //Variables ifstream utilizadas para leer los archivos.txt de usuarios y partidas 
	bool ok = true;

	archivopart.open("partidas.txt"); //Abrimos los archivos .txt de partidas y usuarios
	archivousu.open("usuarios.txt");
	if (archivopart.is_open() && archivousu.is_open()) {  //En caso de que estos se hayan podido abrir correctamente procedemos a su carga en las listas
		archivousu >> gestor.usuarios.contador;
		gestor.usuarios.dimensionUsuarios = gestor.usuarios.contador+1;			
		gestor.usuarios.listausuarios = new tUsuario[gestor.usuarios.dimensionUsuarios];		
		ok = cargar(gestor.usuarios, archivousu);		
		switch (ok) {
		case true: archivopart >> gestor.partidas.contador;
			gestor.partidas.dimensionPartidas = gestor.partidas.contador + 1;
			gestor.partidas.partida = new tPartida[gestor.partidas.dimensionPartidas];
			ok = cargar(gestor.partidas, archivopart); 
			if (!ok) cout << "El archivo 'partidas' ha dado problemas"; 			
			break;
		case false: cout << '\n' << "El archivo 'usuarios' ha dado problemas " << '\n'; 
			break;
		}		
		archivopart.close();  //Cerranmos los archivos .txt después de su carga
		archivousu.close();
		detectarPartidasTerminadas(gestor);
	}
	else ok = false;  //Actualizamos la variable 'bool ok' para informar de si la carga se ha realizado con éxito, o no.
	
	return ok;
}

void generarAccesos(tGestor &gestor) { //Recorre la lista de partidas, y para las que están en curso, añaden el id y el índice a la lista de partidas en curso de cada uno de los adversarios
	tParIdEn part;	
	gestor.listaAcceso.contador = 0;

	for (int contador = 0; contador < gestor.partidas.contador; contador++) { //Recorremos la lista de partidas para buscar aquellas que están en curso
		if (gestor.partidas.partida[contador]->estadoPartida == EnCurso && (gestor.partidas.partida[contador]->jugador1 == gestor.usuarios.listausuarios[gestor.posusu]->nombre || gestor.partidas.partida[contador]->jugador2 == gestor.usuarios.listausuarios[gestor.posusu]->nombre)) {
			part.enlace = gestor.partidas.partida[contador];			
			part.idpart = gestor.partidas.partida[contador]->id;
			gestor.listaAcceso.elementos[gestor.listaAcceso.contador] = part;	
			gestor.listaAcceso.contador++;
		}
	}
}

void apagar(tGestor &gestor) { // Apaga el gestor, abriendo los ficheros para guardar las listas de usuarios y de partidas del sistema
ofstream archivopart, archivousu;

archivousu.open("usuarios.txt");  //Abrimos los archivos .txt en los que vamos a guardar las listas y procedemos a guardarlas
archivopart.open("partidas.txt");

detectarPartidasTerminadas(gestor);

apagar(gestor.partidas, archivopart);
apagar(gestor.usuarios, archivousu);

archivousu.close();  //Cerramos los archivos .txt
archivopart.close();

}

void detectarPartidasTerminadas(tGestor &gestor){ //Función que recorre el array de la lista de partidas, buscando aquellas que estén terminadas (dependiendo de la fecha)
	int pos1, pos2;
	gestor.partidas.dimensionPartidas = gestor.partidas.contador;	
	for (int i = 0; i < gestor.partidas.contador; i++) {			
		if ((fechaActual() - gestor.partidas.partida[i]->fechaActu) >= tFecha(30*24*60*60)) {	 //Si han pasado más de 30 días en una partida:
			gestor.partidas.partida[i]->estadoPartida = Terminada;		 //Pone el estado de la partida aTerminada						
			gestor.partidas.partida[i]->estadoJuego.estado = Ganador;
			gestor.indpart = gestor.partidas.partida[i];
			pos1 = gestor.usuarios.contador;
			pos2 = gestor.usuarios.contador;
			buscar(gestor.usuarios, gestor.partidas.partida[i]->jugador1, pos1); //busca la posición de los jugadores de la partida
			buscar(gestor.usuarios, gestor.partidas.partida[i]->jugador2, pos2);
			gestor.posusu = pos1;				
			if (esSuTurno(gestor)) {		 //Si es el turno del jugador de pos1:		
				aplicarFinPartida(gestor.usuarios.listausuarios[pos1], Pierde);		 //Se declara al jugador de pos1 como perdedor, 
				aplicarFinPartida(gestor.usuarios.listausuarios[pos2], Gana);		 // y al otro como ganador		
				mensajePartidaDetectada(gestor.usuarios.listausuarios[pos2], gestor.usuarios.listausuarios[pos1]); //muestra el mensaje a los dos jugadores, con su correspondiente notificación
			}
			else { //Si no era el turno del jugador de pos1:
				aplicarFinPartida(gestor.usuarios.listausuarios[pos1], Gana); //Se declara al jugador de pos1 como el ganador,  
				aplicarFinPartida(gestor.usuarios.listausuarios[pos2], Pierde); //y al otro como perdedor
				mensajePartidaDetectada(gestor.usuarios.listausuarios[pos1], gestor.usuarios.listausuarios[pos2]); //muestra el mensaje a los dos jugadores, con su correspondiente notificación
			}
			switch (gestor.partidas.partida[i]->estadoJuego.turno) { //detecta quién tenía el turno en último lugar.
			case 10: gestor.partidas.partida[i]->estadoJuego.turno = Jugador1;
				break;
			case 12: gestor.partidas.partida[i]->estadoJuego.turno = Jugador2;
				break;
			}
		}
	}		
}

void mensajePartidaDetectada(tUsuario ganador, tUsuario perdedor) { //Mensaje que aparece cuando una partida de más de 30 días
																	//pasa a estar en el estado Terminada (por inactividad)
	string aviso;
	aviso = "Ha ganado una partida contra " + perdedor->nombre + " por inactividad del contrincante"; //Al ganador informa de que ha ganado (por inactividad del contrincante)
	actualizarAvisos(ganador, aviso);
	aviso = "Ha perdido una partida contra " + ganador->nombre + " por inactividad"; //Así mismo, al perdedor se le informa de que ha perdido por inactividad
	actualizarAvisos(perdedor, aviso);
}

void redimensionarLista(tGestor &gestor) { //Función que cambia la dimensión del array de la lista de Partidas.
	int nuevaCapacidad;
	nuevaCapacidad = (gestor.partidas.dimensionPartidas * 3) / 2 + 1;

	tPartida *aux = new tPartida[nuevaCapacidad];
	for (int i = 0; i < gestor.partidas.dimensionPartidas; i++) {
		aux[i] = gestor.partidas.partida[i];
	}
	delete[] gestor.partidas.partida;
	gestor.partidas.partida = aux;

	gestor.partidas.dimensionPartidas = nuevaCapacidad;
}

void redimensionarUsuarios(tGestor &gestor) {//Función que cambia la dimensión del array de la lista de Usuarios.
	int nuevaCapacidad;
	nuevaCapacidad = (gestor.usuarios.dimensionUsuarios * 3) / 2 + 1;

	tUsuario *aux = new tUsuario[nuevaCapacidad];
	for (int i = 0; i < gestor.usuarios.dimensionUsuarios; i++) {
		aux[i] = gestor.usuarios.listausuarios[i];
	}
	delete[] gestor.partidas.partida;
	gestor.usuarios.listausuarios = aux;

	gestor.usuarios.dimensionUsuarios = nuevaCapacidad;
}

//FUNCIONES QUE USAN EL MENU REGISTRO DEL MAIN

bool iniciarSesion(tGestor &gestor, const string &idUsu, const string &clave) {  //Función que lleva a cabo el inicio de sesión de un usuario dado su nombre y su clave
	bool correcto, encontrado;
	int pos = gestor.usuarios.contador;
	
	encontrado = buscar(gestor.usuarios, idUsu, pos); //Determinamos si el usuario existe, y en caso afirmativo indicamos su posición (pos)

	switch (encontrado) { //En caso de que sea encontrado comprobamos su clave
	case true: correcto = iniciarSesion(gestor.usuarios.listausuarios[pos], clave);
		if (!correcto) {
		cout << '\n' << "Error. Clave incorrecta. " << '\n';
		system("pause");
	}
		else gestor.posusu = pos; //Si todo es correcto seleccionamos al usuario como usuario seleccionado
		cout << '\n' << "Ha iniciado sesion correctamente. " << '\n'; //Informamos de que la sesión se ha iniciado correctamente
			break;
	case false: correcto = false;
		cout << '\n' << "Error. El usuario no existe. " << '\n';  //Informamos de que el usuario no existe
		system("pause"); break;
	}
	if (correcto) {
		generarAccesos(gestor);
	}
		return correcto;
}

bool crearCuenta(tGestor &gestor, const string &idUsu, const string &clave) {  //Función que crea una cuenta nueva dado un nombre de usuario y una contraseña
	bool sePuede = true, encontrado; 
	int  indice = 0;
	gestor.posusu = 0;
	int pos = gestor.usuarios.contador;

	encontrado = buscar(gestor.usuarios, idUsu, pos); //Buscamos si existe ya el usuario que se desea crear
		
	do {
		if (encontrado)
			sePuede = false;
		else gestor.posusu++;
	} while (!encontrado && gestor.posusu < gestor.usuarios.contador); //Buscamos la posicion en la que se debera insertar el usuario nuevo para conservar el orden de la lista
	
	switch (sePuede) {  //En caso de que todo sea correcto procedemos a crear este nuevo usuario e insertarlo en su posicion correspondiente
	case true: if (gestor.usuarios.contador >= gestor.usuarios.dimensionUsuarios) {

	}
		gestor.usuarios.listausuarios[gestor.posusu] = new tUsu;
		iniciar(gestor.usuarios.listausuarios[gestor.posusu], idUsu, clave);
		insertar(gestor.usuarios, gestor.usuarios.listausuarios[gestor.usuarios.contador], gestor.posusu);
		gestor.usuarios.contador++; break;
	case false: cout << '\n' << "El usuario ya existe." << '\n';
			system("pause"); break;
	}
	if (sePuede) {
		gestor.listaAcceso.contador = 0;
	}

	return sePuede;
}

bool tieneAvisos(const tGestor &gestor, string &aviso) { //Función que determina si el usuario tiene avisos
	bool tiene = true; //Variable bool que nos servirá para determinar si tiene o no avisos (el usuario)
	ostringstream aux; //Flujo auxiliar que nos servirá para determinar de una forma mas sencilla si tiene avisos el usuario
	aux << '\n';
	if (gestor.usuarios.listausuarios[gestor.posusu]->avisos != aux.str()) { //Determinamos si tiene o no avisos el usuario
		cout << '\n' << "Tiene avisos: " << '\n';
		aviso = gestor.usuarios.listausuarios[gestor.posusu]->avisos;
	}
	else tiene = false;

	return tiene;
}

void limpiarAvisos(tGestor &gestor) {  //Función que limpia los avisos del usuario borrando todos
	limpiarAvisos(gestor.usuarios.listausuarios[gestor.posusu]);
}

string resumenActividad(const tGestor &gestor) { //Función que devuelve un resumen de la actividad del usuario (racha, ganadas, perdidas,...)
	ostringstream actividad; //Flujo que nos servirá para devolver el resumen de actividad del usuario

	actividad << "Partidas ganadas: " << gestor.usuarios.listausuarios[gestor.posusu]->ganadas << '\n';
	actividad << "Partidas perdidas: " << gestor.usuarios.listausuarios[gestor.posusu]->perdidas << '\n';
	actividad << "Partidas empatadas: " << gestor.usuarios.listausuarios[gestor.posusu]->empatadas << '\n';
	actividad << "Nivel: ";

	switch (gestor.usuarios.listausuarios[gestor.posusu]->nivel) { //Convertimos el tipo tNivel a un string que podamos mostrar por pantalla fácilmente
	case Principiante: actividad << "Principiante "; break;
	case Medio: actividad << "Medio "; break;
	case Experto: actividad << "Experto "; break;
	}

	actividad << '\n' << "Racha: ";

	if (gestor.usuarios.listausuarios[gestor.posusu]->racha > 0) //Informamos sobre el usuario: si lleva una racha de partidas ganadas, perdidas o ninguna racha
		actividad << gestor.usuarios.listausuarios[gestor.posusu]->racha << " ganadas";
	else if (gestor.usuarios.listausuarios[gestor.posusu]->racha == 0)
		actividad << "No lleva ninguna racha actualmente (0)";
	else actividad << -gestor.usuarios.listausuarios[gestor.posusu]->racha << " perdidas";

	actividad << '\n';

	if (gestor.usuarios.listausuarios[gestor.posusu]->esperando) //Informamos sobre si el usuario esta esperando una partida o no
		actividad << "Esta esperando actualmente un adversario para jugar una nueva partida ";
	else actividad << "No esta esperando ningun adversario";
	actividad << '\n';

	return actividad.str(); //Devolvemos el resumen de actividad
}

void mostrarActividad(const tGestor &gestor) {  //Función que se encarga de mostrar por pantalla la actividad del usuario
	ostringstream actividad;

	actividad << resumenActividad(gestor);
	cout << '\n' << "ACTIVIDAD:" << '\n' << '\n' << actividad.str();  //Mostramos por pantalla la actividad
}

//FUNCIONES DEL MENÚ USUARIO
int partidasUsuario(const tGestor &gestor) {  //Función que devuelve el número de partidas en curso que tiene el usuario
	return gestor.listaAcceso.contador;
}

string cabecera(const tGestor &gestor) {  //Función que muestra la información de una partida del usuario
	ostringstream informacion;
	bool hora = true;

	if (esSuTurno(gestor))	informacion << "  *" << "       ";  //Comprobamos si es o no el turno del usuario, para mostrarlo con '*'
	else informacion << "          ";

	informacion << setw(15) << left << gestor.indpart->jugador1 << setw(15) << left << gestor.indpart->jugador2 << stringFecha(gestor.indpart->fechaActu, hora) << '\n' << '\n';

	return informacion.str();
}

void ordenar_Fecha(tGestor &gestor) { //Función que ordena las partidas de un usuario por fecha
tParIdEn aux;
bool inter = true;
int i = 0;
while ((i < gestor.listaAcceso.contador - 1) && inter) {
	inter = false;
	// Desde el último hasta el siguiente a i...
	for (int j = gestor.listaAcceso.contador - 1; j > i; j--) {
		if (gestor.listaAcceso.elementos[j].enlace->fechaActu < gestor.listaAcceso.elementos[j-1].enlace->fechaActu) {
			tParIdEn tmp;
			tmp = gestor.listaAcceso.elementos[j];
			gestor.listaAcceso.elementos[j] = gestor.listaAcceso.elementos[j-1];
			gestor.listaAcceso.elementos[j-1] = tmp;
			inter = true;
		}
	}
		if (inter) {
		i++;
		}
	}
}

void ordenar_Turno(tGestor &gestor) { //Función que ordena las partidas de un usuario por turno
	int pos;
	tParIdEn nuevo;
	
	for (int i = 1; i < gestor.listaAcceso.contador; i++) { //Realizamos un recorrido de la lista de usuarios		
		apuntaPartida(gestor, i);
		if (esSuTurno(gestor)) {  //En caso de que en una partida el usuario tenga el turno, revisamos si las partidas anteriores también tienen el turno
			pos = 0;			
			apuntaPartida(gestor, pos);
			while (pos < i && esSuTurno(gestor)) { //Buscamos el primer usuario que no tenga el turno (si lo hay)
				pos++;
				apuntaPartida(gestor, pos);
			}			
			nuevo = gestor.listaAcceso.elementos[i];
			for (int j = i; j > pos; j--) {  //Reordenamos la lista de partidas en caso de que sea necesario
				gestor.listaAcceso.elementos[j] = gestor.listaAcceso.elementos[j-1];
			}
			gestor.listaAcceso.elementos[pos] = nuevo;
		}
	}
}

bool nuevaPartida(tGestor &gestor) { //Función que crea una nueva partida
	bool encontrado, ok;
	int pos = gestor.posusu;
	int contador = partidasUsuario(gestor);
	tParIdEn datos;
	ostringstream aviso;


	if (!gestor.usuarios.listausuarios[gestor.posusu]->esperando && contador < MAX_PAR_US) { //Determinamos si el usuario está ya esperando una partida o si ha alcanzado el máximo número de partidas permitido
		encontrado = buscarUsuarioEsperando(gestor.usuarios, gestor.usuarios.listausuarios[gestor.posusu]->nivel, pos); //Buscamos un usuario esperando del mismo nivel

		if (encontrado) { //Si este es encontrado
			if (gestor.partidas.contador >= gestor.partidas.dimensionPartidas) {
				redimensionarLista(gestor);
			}
			gestor.partidas.partida[gestor.partidas.contador] = new tPart;
			nueva(gestor.partidas.partida[gestor.partidas.contador], gestor.usuarios.listausuarios[gestor.posusu]->nombre, gestor.usuarios.listausuarios[pos]->nombre);			
			aviso << "Se ha creado una nueva partida contra " << gestor.usuarios.listausuarios[gestor.posusu]->nombre << " en la fecha " << stringFecha(fechaActual(), false);
			actualizarAvisos(gestor.usuarios.listausuarios[pos], aviso.str());
			gestor.usuarios.listausuarios[gestor.posusu]->esperando = false;			
			gestor.usuarios.listausuarios[pos]->esperando = false;
			gestor.listaAcceso.elementos[gestor.listaAcceso.contador].enlace = gestor.partidas.partida[gestor.partidas.contador];
			gestor.listaAcceso.elementos[gestor.listaAcceso.contador].idpart = gestor.partidas.partida[gestor.partidas.contador]->id;
			gestor.partidas.contador++;
			generarAccesos(gestor);
			ok = true;
		}
		else { // si no se encuentra ningún jugador esperando del mismo nivel
			gestor.usuarios.listausuarios[gestor.posusu]->esperando = true;
			cout << '\n' << "Estas esperando a un adversario para jugar una partida" << '\n';
			ok = false;
		}
	}
	else {  //En caso de haber alcanzado el máximo de partidas en curso
		ok = false;	
		cout << '\n' << "Te has excedido con el numero de partidas en curso o bien ya estas esperando a un adversario" << '\n' << '\n';			
	}

	
	return ok;
}

void apuntaPartida(tGestor &gestor, int posParEnCurso) {  //Función que actualiza el campo gestor.indpart, indicando cuál es la partida seleccionada por el usuario
	gestor.indpart = gestor.listaAcceso.elementos[posParEnCurso].enlace;	
}

//FUNCIONES DEL MENÚ PARTIDA
void mostrarPartida(const tGestor &gestor) {  //Función que muestra una partida 
	mostrar(gestor.indpart->estadoJuego, gestor.indpart->jugador1, gestor.indpart->jugador2);
}

bool esSuTurno(const tGestor &gestor) {  //Función que determina si es el turno del usuario seleccionado en una partida
	 bool turno; 
	 //Comprobamos si es el turno del usuario en la partida seleccionada
	 if ((gestor.usuarios.listausuarios[gestor.posusu]->nombre == gestor.indpart->jugador1 && gestor.indpart->estadoJuego.turno == Jugador1) ||
		 (gestor.usuarios.listausuarios[gestor.posusu]->nombre == gestor.indpart->jugador2 && gestor.indpart->estadoJuego.turno == Jugador2))
		 turno = true;
	 else turno = false;

	 return turno;
}

void jugarPartida(tGestor &gestor, int col) { //Función que, dado un movimiento, aplica la jugada y actualiza los adversarios (si es necesario)
	int pos = gestor.usuarios.contador - 1;
	bool encontrado = false;
	bool hora = false;
	string adversario;
	ostringstream aviso;
	if (gestor.usuarios.listausuarios[gestor.posusu]->nombre == gestor.indpart->jugador1) //Determinamos si el adversario es el jugador1 o el jugador2
		adversario = gestor.indpart->jugador2;
	else adversario = gestor.indpart->jugador1;

	buscar(gestor.usuarios, adversario, pos); //Busacmos al adversario en la lista de partidas para determinar su posicion (pos)

	if (aplicarJugada(gestor.indpart, col)) { //En caso de que se haya podio aplicar la jugada:
		switch (gestor.indpart->estadoJuego.estado) {  //En función del estado del juego, actualizamos los datos de la partida
		case Ganador: 	gestor.indpart->estadoPartida = Terminada;
			aplicarFinPartida(gestor.usuarios.listausuarios[gestor.posusu], Gana); //El usuario de la sesión gana
			aviso << "Ha perdido la partida contra " << gestor.usuarios.listausuarios[gestor.posusu]->nombre << " el " << stringFecha(gestor.indpart->fechaActu, hora);
			actualizarAvisos(gestor.usuarios.listausuarios[pos], aviso.str());
			aplicarFinPartida(gestor.usuarios.listausuarios[pos], Pierde); // El adversario, en cambio, pierde
			break;
		case Bloqueo:	gestor.indpart->estadoPartida = Terminada;
			aplicarFinPartida(gestor.usuarios.listausuarios[gestor.posusu], Empata); //en estado de Bloqueo ambos empatan
			aviso << "Ha empatado la partida contra " << gestor.usuarios.listausuarios[gestor.posusu]->nombre << " el " << stringFecha(gestor.indpart->fechaActu, hora);
			actualizarAvisos(gestor.usuarios.listausuarios[pos], aviso.str());
			aplicarFinPartida(gestor.usuarios.listausuarios[pos], Empata);
			break;
		}
		eliminarPartidaTerminada(gestor, gestor.indpart->id);
	}
	else cout << '\n' << "No se pudo aplicar la jugada." << '\n';  //Informamos de que no se pudo aplicar la jugada
}

void eliminarPartidaTerminada(tGestor &gestor, const string &id) {//Función que se encarga de eliminar las partidas ya terminadas de la lista de Partidas
	bool eliminada = false;
	int i = 0;

	eliminada = buscar(gestor.listaAcceso, id, i); 

	if (eliminada) {
		for (int j = i; j < gestor.listaAcceso.contador - 1; j++) {
		gestor.listaAcceso.elementos[j] = gestor.listaAcceso.elementos[j + 1];
		}
		gestor.listaAcceso.contador--;
	}
}

void abandonarPartida(tGestor &gestor) {  //Función que aplica la decisión del usuario de abandonar una partida
	int pos;
	bool hora = false;
	string adversario;
	ostringstream aviso;

	if (gestor.usuarios.listausuarios[gestor.posusu]->nombre == gestor.indpart->jugador1) //Determinamos quién es el adversario
		adversario = gestor.indpart->jugador2;
	else adversario = gestor.indpart->jugador1;

	if (adversario > gestor.usuarios.listausuarios[gestor.posusu]->nombre)
		pos = gestor.usuarios.contador;		//Para que la posterior búsqueda tenga que dar menos vueltas si el adversario está después del usuario de la sesión
	else pos = gestor.posusu;

		buscar(gestor.usuarios, adversario, pos); //Busca la posición del adversario en la lista de usuarios	
		abandonar(gestor.indpart); //Abandona la partida, cambiando el estado a Terminada
		//Cambia los datos de los usuarios según hayan ganado o perdido
		if (gestor.usuarios.listausuarios[gestor.posusu]->nombre == gestor.indpart->jugador1)
			gestor.indpart->estadoJuego.turno = Jugador2;
		else gestor.indpart->estadoJuego.turno = Jugador1;
		gestor.indpart->estadoJuego.estado = Ganador;
		aplicarFinPartida(gestor.usuarios.listausuarios[gestor.posusu], Pierde);
		aplicarFinPartida(gestor.usuarios.listausuarios[pos], Gana);
		gestor.indpart->estadoPartida = Terminada;
		eliminarPartidaTerminada(gestor, gestor.indpart->id);
		aviso << "Ha ganado por abandono una partida contra " << gestor.usuarios.listausuarios[gestor.posusu]->nombre << " el " << stringFecha(gestor.indpart->fechaActu, hora) << ".";
		actualizarAvisos(gestor.usuarios.listausuarios[pos], aviso.str()); //Deja un aviso (de que ha ganado la partida) en la sesión del adversario
		cout << '\n' << "Ha elegido abandonar la partida" << '\n'; //Informamos de que se ha decidido abandonar la partida
		system("pause");
}

bool buscar(tListaAccesoPartidas const &listaAcceso, string const &id, int &pos){ //De forma recursiva, buscamos en la lista de accesos a partida el ID de la partida de la cual deseamos conocer su posición
bool eliminada = false;

	if (listaAcceso.elementos[pos].enlace->id != id){
		if (pos < listaAcceso.contador) {
			pos = pos++;
			eliminada = buscar(listaAcceso, id, pos);
		}
	}
	else eliminada = true;

return eliminada; //Devolvemos si ha sido posible determinar correctamente la posición de la partida
}
