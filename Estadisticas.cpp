// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12
#include "Estadisticas.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void cargarEstadisticas(tListaEstadisticas &lista, string usuario) { //Funci�n que carga las estad�sticas del archivo "historico.txt"
	ifstream archivo;	
	string auux;
	string aux1;
	string aux2;
	string adversario;
	int pos = 0;
	int aux;
	int auxiliar;		

		archivo.open("historico.txt"); //Primero, abre el archivo.
		if (archivo.is_open()) { //Si este se ha abierto correctamente, carga todas las estad�sticas en la lista de las estad�sticas
			archivo >> lista.tama�oEstadisticas;			
			lista.listaEstadisticas = new tEstadisticas[lista.tama�oEstadisticas];
			lista.contador = 0;
			getline(archivo, auux);
			
			for (int i = 0; i < lista.tama�oEstadisticas; i++) {				
				getline(archivo, auux); //primero, coge el id de la partida
				archivo >> aux1; //coge el nombre del jugador 1
				archivo >> aux2; //coge el nombre del jugador 2

				if (aux1 == usuario || aux2 == usuario) {  //si alguno de los dos nombres coincide con el del usuario de la sesi�n:

					if (aux1 == usuario) adversario = aux2; //Si el usuario 1 es el jugador1, el adversario ser� el jugador2
					else adversario = aux1; //si no, el adversario ser� el jugador1

					if (!existeElAdversario(lista, adversario, pos)) { //Comprueba si ya ha guardado previamente a dicho adversario en la lista de estad�sticas.
																		//Si no lo ha guardado, lo guarda en la primera posici�n vac�a que haya. Si est�, devuelve la posici�n del adversario en la lista.
						pos = lista.contador;
						lista.listaEstadisticas[pos] = new tEstadistica;
						iniciarEstadistica(lista.listaEstadisticas[pos], adversario);
						lista.contador++;
					}
					//Sigue cogiendo datos del archivo 
					getline(archivo, auux); //Coge las fechas (no las necesitamos): Fecha del inicio de la partida
					getline(archivo, auux); //Fecha de la �ltima actualizaci�n de la partida
					archivo >> aux;//Coge el estado de la partida
					archivo >> aux; //Coge el estado del juego
					archivo >> auxiliar; //auxiliar coge qui�n ten�a el turno al terminar la partida

					switch (aux) {
					case 2: lista.listaEstadisticas[pos]->empatadas++; //En el caso de que sea 2, significa que la partida ha terminado con el estado Bloqueo
						break;
					case 1: //En el caso de que sea uno (Ganador), significa que ha habido un ganador
						if ((aux1 == adversario && auxiliar == 10) || (aux2 == adversario && auxiliar == 12))  lista.listaEstadisticas[pos]->ganadas++; //Se comprueba el turno para ver qui�n ha perdido
						else lista.listaEstadisticas[pos]->perdidas++;
						break;
					}
					for (int i = 0; i < 7; i++) { //Coge el n�mero de fichas, y la informaci�n del tablero (no lo necesitamos para nada)
						getline(archivo, auux);
					}
				}
				else { //si no, pasa por el archivo (cogiendo datos mediante auxiliares) hasta llegar a la siguiente partida
					for (int i = 0; i < 10; i++) {
						getline(archivo, aux1);
					}
				}
			}
		}
		else { //Si no existe, pone el contador de las estad�sticas a 0, y el tama�o del array a 1.
			lista.tama�oEstadisticas = 1;
			lista.listaEstadisticas = new tEstadisticas[lista.tama�oEstadisticas];
			lista.contador = 0;
		}
		archivo.close(); //Se cierra el archivo
}

bool existeElAdversario(const tListaEstadisticas &lista, const string adversario, int &pos) { //Funci�n que comprueba si ya ha guardado previamente a dicho adversario en la lista de estad�sticas
	bool existe = false;
    int i = 0;	
	if (lista.contador != 0) {		
		while (i < lista.contador && !existe) { //Recorre comprobando si el nombre del adversario est� en el array.
			if (lista.listaEstadisticas[i]->adversario == adversario) {
				existe = true; //Si lo encuentra, existe cambia a 'true' 
			}
			i++;
		}		
		//i no lo ha encontrado, devolver� la primera posici�n vac�a que haya. Si est� en la lista, devuelve la posici�n del adversario en dicha lista.
		pos = i - 1;
	}
	return existe; 
}

void iniciarEstadistica(tEstadisticas estadistica, string adversario) { //Funci�n que crea un nuevo adversario en la lista
	estadistica->adversario = adversario; //Guarda en 'estadistica->adversario' el nombre que se le pasa.
	estadistica->empatadas = 0;
	estadistica->ganadas = 0;
	estadistica->perdidas = 0; //Pone todos los campos de las estad�sticas a 0
}

void apagarEstadisticas(tListaEstadisticas &lista) { //Funci�n que elimina el array (de datos din�micos) que guarda la lista de estad�sticas
	for (int i = 0; i < lista.contador; i++) {	
		delete lista.listaEstadisticas[i];		
	}		
	delete[] lista.listaEstadisticas;		
}

void actualizarEstadisticas(tListaEstadisticas &lista, string usuario, const tListaPartidas &partidas, tListaIdExistentes &listaEx) { //Funci�n que actualiza la lista de estad�sticas
	string adversario;
	int pos = 0;		

	if (lista.contador == lista.tama�oEstadisticas) { //Si hay tantos elementos como espacio en el array se redimensiona
		redimensionarEstadisticas(lista);
	}
		for (int i = 0; i < partidas.contador; i++) {//Recorre todos los elementos de la lista de partidas

			if (partidas.partida[i]->estadoPartida == Terminada && (partidas.partida[i]->jugador1 == usuario || partidas.partida[i]->jugador2 == usuario) //Si el estado de la partida es Terminada,
				&& !buscarPartidaExistente(listaEx, partidas.partida[i]->id)) {//el nombre de alguno de los jugadores coincide con el del usuario, y no est� en la lista de partidas existentes:
				
				listaEx.idExistente[listaEx.contador] = partidas.partida[i]->id; //guarda el id de la partida en la lista de partidas existentes
				listaEx.contador++; //Incrementa el contador

				if (partidas.partida[i]->jugador1 != usuario) adversario = partidas.partida[i]->jugador1; //Si el nombre del jugador1 no coincide con el del usuario, el adversario ser� el jugador1
				else adversario = partidas.partida[i]->jugador2;//Si no, el adversario ser� el jugador 2

				if (!existeElAdversario(lista, adversario, pos)) {	 //Si no est� el adversario a�n en la lista de estad�sticas, se le a�ade				
					pos = lista.contador;
					lista.listaEstadisticas[pos] = new tEstadistica;
					iniciarEstadistica(lista.listaEstadisticas[pos], adversario);
					lista.contador++;
				}					
				switch (partidas.partida[i]->estadoJuego.estado) {
				case 2: lista.listaEstadisticas[pos]->empatadas++; //Si el estado del juego es Bloqueo, significa que ha habido un empate (se incrementan las empatadas)
					break;
				case 1: //En cambio, si el estado es Terminada, significa que ha habido un ganador
					if ((partidas.partida[i]->jugador1 == adversario && partidas.partida[i]->estadoJuego.turno == 10) ||
						(partidas.partida[i]->jugador2 == adversario && partidas.partida[i]->estadoJuego.turno == 12))  lista.listaEstadisticas[pos]->ganadas++; //Si el adversario ten�a el turno, es el perdedor, y se incrementan las ganadas del usuario de la sesi�n.
					else lista.listaEstadisticas[pos]->perdidas++; //Si no, se incrementan el n�mero de partidas perdidas del usuario de la sesi�n (contra dicho adversario)
					break;
				}
			}
		}	
}

bool buscarPartidaExistente(const tListaIdExistentes &lista, const string id) { //Funci�n que comprueba si la partida existe
	int i = 0;
 	bool existe = false;

	while (i < lista.contador && !existe) {
		if (lista.idExistente[i] == id) existe = true; //Comprueba uno por uno los id de las partidas existentes con el string que se le pasa
														//a la funci�n hasta que encuentre a alguno, o pase por todos los elementos de la lista.
		i++;
	}
	return existe;
}

void redimensionarEstadisticas(tListaEstadisticas &lista) {	//Funci�n que sirve para redimensionar el array de la lista de estad�sticas.
	int nuevaCapacidad;
	nuevaCapacidad = (lista.tama�oEstadisticas * 3) / 2 + 1;	
	tEstadisticas *aux = new tEstadisticas[nuevaCapacidad];	
	for (int i = 0; i < lista.tama�oEstadisticas; i++) {
		aux[i] = lista.listaEstadisticas[i];		
	}
	delete[] lista.listaEstadisticas;
	lista.listaEstadisticas = aux;
	lista.tama�oEstadisticas = nuevaCapacidad;	
}

ostringstream mostrarEstadisticas(const tListaEstadisticas &lista) { //Muestra la lista de las estad�sticas del usuario de la sesi�n.
	ostringstream flujo;
	int pos = 0; //Pone la posici�n a 0, para recorrer la lista de estad�sticas desde 0
	system("CLS");
	flujo << "ESTADISTICAS POR ADVERSARIO" << '\n' << "---------------------------" << '\n' << '\n' << endl;
	if (lista.contador > 0) {
		actualizarFlujoEstadisticas(lista, flujo, pos); //Mostrar� todos los datos de las estad�sticas del usuario de la sesi�n (desde la posici�n 0, que se le pasa a la funci�n)
	}
	else flujo << "Aun no tienes estadisticas contra ningun adversario..." << '\n' << '\n' << endl;
	flujo << "---------------------------" << '\n' << '\n' << endl;	
	return flujo;
}

void actualizarFlujoEstadisticas(const tListaEstadisticas &lista, ostringstream &flujo, int &pos) { //Funci�n que actualiza el flujo con los datos de las estad�sticas
	flujo << "ADVERSARIO: " << lista.listaEstadisticas[pos]->adversario << '\n' << endl; //El flujo primero coge el nombre del adversario.
	flujo << "  Ganadas:   " << lista.listaEstadisticas[pos]->ganadas << '\n' << endl; //Despu�s coge el n�mero de partidas ganadas contra dicho adversario.
	flujo << "  Empatadas: " << lista.listaEstadisticas[pos]->empatadas << '\n' << endl; //Posteriormente, coge el n�mero de partidas empatadas.
	flujo << "  Perdidas:  " << lista.listaEstadisticas[pos]->perdidas << '\n' << '\n' << endl; //Por �ltimo, coge el n�mero de partidas perdidas contra dicho adversario.
	pos++; //pasa a la siguiente posici�n
	if (pos < lista.contador) { //Mientras que la posici�n que se le pasa sea menor que los elementos de la lista:
		actualizarFlujoEstadisticas(lista,flujo, pos); //seguir� cogiendo datos el flujo
	}
}