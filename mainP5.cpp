// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "Gestor.h"
#include "Estadisticas.h"
#include "checkML.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

void menuRegistro(tGestor &gestor); //Función que controla el desarrollo del menú registro

void mostrarDatosUsu(tGestor &gestor); //Función que muestra por pantalla los datos del usuario

void menuUsuario(tGestor &gestor);  //Función que muestra y controla el menú usuario

void mostrarMenuReg(); //Función que muestra por pantalla el menú registro

void mostrarPartidasEnCurso(tGestor &gestor); //Función que muestra por pantalla las partidas que el usuario tiene en curso

void elegirPartida(tGestor &gestor); //Función que nos permite elegir la partida que deseamos ver

void leerJugada(int &col); //Función que se encarga de leer la jugada

void menuPartida(tGestor &gestor);  //Función que controla el menú de la partida

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Comando que sirve para muestrar información sobre la posible
																// basura que ha podido dejar el programa al terminar la ejecución.
	tGestor gestor;
	string aviso;
	
	bool ok;	
	ok = arrancar(gestor); //Realizamos el arranque del gestor y comprobamos si se ha realizado correctamente		
	if (ok) {
		menuRegistro(gestor); //Si el gestor se ha iniciado correctamente, accedemos al menú registro
		apagar(gestor); //Apagamos el gestor
	}
	return 0;
}

void menuRegistro(tGestor &gestor) { //Función que controla el desarrollo del menú registro
	int opcion;	
	string nombre, clave;

	do {  //Nos mantendremos en este menú mientras que la opción elegida sea distinta de 0 (salir)
		mostrarMenuReg();		
		cin >> opcion;
		while (opcion < 0 || opcion > 2) {  //Comprobamos que se esté introduciendo una opción posible
			cout << '\n' << "Por favor, introduzca una opcion valida: ";
			cin >> opcion;
		}

		if (opcion != 0) {  //En caso de que la opción sea distinta de 0, solicitamos el nombre de usuario y la clave para acceder a una sesión, o crear una cuenta nueva
			system("cls");
			cout << '\n' << '\n' << "ACCESO" << endl;
			cout << '\n' << "Nombre de usuario: ";
			cin >> nombre;
			cout << "Clave: ";
			cin >> clave;
		}
		if (opcion == 1 && iniciarSesion(gestor, nombre, clave)) {	//En caso de que la opción seleccionada sea '1', procedemos al inicio de sesión	
			system("PAUSE");
			system("CLS");
			mostrarDatosUsu(gestor);
			system("PAUSE");
			system("CLS");
			menuUsuario(gestor);
			system("PAUSE");			
		}
		if (opcion == 2 && crearCuenta(gestor, nombre, clave)) { //En caso de que la opción sea '2', procedemos a crear una cuenta con el nombre de usuario y clave proporcionada
			cout << '\n' << "Ha creado la cuenta correctamente" << '\n';
			system("pause");
			system("cls");
			mostrarDatosUsu(gestor);
			system("pause");
			system("CLS");
			menuUsuario(gestor);
			system("pause");			
		}			
	}while (opcion != 0);
}

void mostrarDatosUsu(tGestor &gestor) { //Función que muestra por pantalla los datos del usuario
	char borraravisos;

	if (tieneAvisos(gestor, gestor.usuarios.listausuarios[gestor.posusu]->avisos)) { //En caso de que tenga avisos, los muestra por pantalla
		cout << '\n' << gestor.usuarios.listausuarios[gestor.posusu]->avisos << '\n' << '\n' << "Elija si desea borrar los avisos: [S/N] ";
		cin >> borraravisos;
		while (borraravisos != 'S' && borraravisos != 'N'&& borraravisos!= 's' && borraravisos!= 'n') { 
			cout << '\n' << "Introduzca una opcion valida ";
			cin >> borraravisos;
		}
		borraravisos = toupper(borraravisos);  //Utilizamos toupper() para así solo tener que comprobar el char de la letra mayúscula
		switch (borraravisos) {		//En caso de que lo solicite, el usuario borra los avisos del usuario
		case 'S': limpiarAvisos(gestor);
			break;		
		case 'N': break;
		}
	}
	system("CLS");
	mostrarActividad(gestor); //Mostramos la actividad del usuario
	cout << '\n' << '\n';
}

void menuUsuario(tGestor &gestor) {  //Función que muestra y controla el menú usuario
	int opcion;	
	tListaEstadisticas lista;
	tListaIdExistentes listaEx;	
	cargarEstadisticas(lista, gestor.usuarios.listausuarios[gestor.posusu]->nombre);
	listaEx.contador = 0;
	do {  //Nos mantendremos en este bucle mientras que la opción elegida no sea '0' (salir)
		system("cls");
		cout << '\n' << "MENU USUARIO" << endl;  //Mostramos el menú de usuario con las partidas en curso del usuario

		mostrarPartidasEnCurso(gestor);

		cout << '\n' << "1. Ver partida ";
		cout << '\n' << "2. Nueva partida ";
		cout << '\n' << "3. Partidas por actualizacion";
		cout << '\n' << "4. Partidas por turno y fecha";
		cout << '\n' << "5. Ver estadisticas por adversario";
		cout << '\n' << "0. Salir";
		cout << '\n' << "Elija una opcion: ";
		cin >> opcion;
		while (opcion < 0 || opcion > 5) {
			cout << '\n' << "Por favor, introduzca una opcion valida: ";
			cin >> opcion;
		}

		switch (opcion) {
		case 1: system("CLS");   //En caso de que el usuario eliga la opción '1' le mostramos las partidas en curso para que eliga cuál quiere que le mostremos
			mostrarPartidasEnCurso(gestor);
			elegirPartida(gestor);
			break;
		case 2: if (nuevaPartida(gestor))   //En caso de que solicite una nueva partida, buscamos un usuario de su nivel y que esté en espera para crear una nueva
		 cout << '\n' << "La partida se ha creado correctamente " << '\n';       //partida; en caso de no encontrarlo, se deja al usuario en espera
		system("pause"); break;
		case 3: ordenar_Fecha(gestor); break;  //Con la opción '3' ordenamos por fecha
		case 4: ordenar_Fecha(gestor); ordenar_Turno(gestor); break;  //En la opción '4' primero ordenamos por fecha la partida, y posteriormente la ordenamos por
		            // turno, para así poder tener esta ordenada por turnos (primero aquellas partidas en las que el usuario de la sesión tiene el turno) y dentro de cada grupo, ordenadas por fecha.
		case 5: actualizarEstadisticas(lista, gestor.usuarios.listausuarios[gestor.posusu]->nombre, gestor.partidas, listaEx); // Actualiza las estadísticas (de partidas terminadas) del usuario de la sesión	
			cout << mostrarEstadisticas(lista).str(); system("pause"); break;				//muestra las estadísticas
		case 0: cout << '\n' << "Ha elegido salir" << '\n'; break;   //Informamos al usuario de que ha elegido salir
		}		
	} while (opcion != 0);	
	apagarEstadisticas(lista);	
}

void mostrarMenuReg() {  //Función que muestra por pantalla el menú registro
	system("CLS");
	cout << '\n' << "MENU REGISTRO";
	cout << '\n' << "1.- Acceder a mi cuenta";
	cout << '\n' << "2.- Registrarse";
	cout << '\n' << "0.- Salir" << '\n';
	
}

void mostrarPartidasEnCurso(tGestor &gestor) { //Función que muestra por pantalla las partidas que el usuario tiene en curso
	ostringstream mostrar;  //Flujo que facilitará el visionado de las partidas en curso del usuario
	int contador = partidasUsuario(gestor);
	
	mostrar << "";	
	cout << '\n' << '\n';
	cout << "#   T      " << "JUGADOR 1" << "      " << "JUGADOR 2" << "       " << "FECHA " << '\n';
	cout << setfill('-') << setw(80) << " " << '\n';
	
	for (int i = 0; i < contador; i++) {  //Realizamos un recorrido por la lista de partidas en curso del usuario añadiendo estas al flujo, para así poder mostrarlas con mayor facilidad
		mostrar << i + 1 << " ";		
		apuntaPartida(gestor, i);		
		mostrar << cabecera(gestor);		
	}
	cout << mostrar.str(); //Mostramos el contenido del flujo
}

void elegirPartida(tGestor &gestor) { //Función que nos permite elegir la partida que deseamos ver
	int ind;	
	int contador = partidasUsuario(gestor); //Actualizamos el contador al número de partidas en curso que tiene el usuario

	cout << '\n' << "Seleccione partida (0 para salir): ";  //Solicitamos la partida
	cin >> ind;
	cout << '\n' << '\n';

	while (ind > contador || ind < 0) {  //Comprobamos que el número de partida introducida es válida
		cout << '\n' << "Introduzca un numero valido: ";
		cin >> ind;
	}
	if(ind != 0) {  //En caso de que la opción no sea '0' (salir), procedemos a mostrar la partida y acceder al menú de la partida 
		ind--;
		apuntaPartida(gestor, ind);		
		system("cls");
		mostrarPartida(gestor);
		menuPartida(gestor);		
	}
	else cout << '\n' << "Ha seleccionado salir. " << '\n';
}

void leerJugada(int &col) {  //Función que se encarga de leer la jugada
	cout << '\n' << "Por favor, introduzca el numero de columna en la que quiere colocar la ficha: ";
	cin >> col;
	while (col > COLS || col < 0) {  //Comprobamos que la columna es válida; en caso contrario, pedimos que introduzca una que sí sea válida
		cout << '\n' << "Introduzca una columna valida:";
		cin >> col;
	}
	col--;
}

void menuPartida(tGestor &gestor) {  //Función que controla el menú de la partida
	int opcion = 1;
	int col = 0;
	bool finpart = false;

	while (opcion != 0 && !finpart) {
		if (gestor.indpart->estadoPartida == EnCurso) { //Mientras que la partida se encuentre en curso, mostraremos las opciones del menú
			cout << '\n' << "1. Jugar ";
			cout << '\n' << "2. Abandonar ";
			cout << '\n' << "0. Salir ";
			cout << '\n' << " Elija una opcion: ";
			cin >> opcion;
			
			while (opcion < 0 || opcion>2) { //Comprobamos que la opción escogida sea válida
				cout << "Elija una opcion valida: ";
				cin >> opcion;
			}
		}
		else {	//Si la partida termina, salimos del menú		
			system("PAUSE");
			finpart = true;
		}
		

		switch (opcion) {
		case 1: if (esSuTurno(gestor) && gestor.indpart->estadoPartida != Terminada) { 
				leerJugada(col);              //En caso de que el usuario desee jugar, comprobamos si es su turno; en caso afirmativo, le permitimos jugar
				jugarPartida(gestor, col);
				system("cls");
				mostrarPartida(gestor);
			}
				else if (!esSuTurno(gestor)) { //Si no es su turno, le informamos de que debe esperar
					system("cls");
					cout << '\n' << "No puedes poner ficha, no es tu turno." << '\n' << '\n';
					mostrarPartida(gestor);
				}				
			break;
		case 2: 
			if (gestor.indpart->estadoPartida != Terminada) { //Permitimos al usuario abandonar la partida cuando esta esté en curso
				abandonarPartida(gestor);
				finpart = true;
			}			
		case 0: cout << '\n' << "Ha elegido salir" << '\n'; break; //Informamos de que el usuario ha decidido salir
		}
	}
}