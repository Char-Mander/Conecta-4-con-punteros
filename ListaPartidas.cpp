// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#include "ListaPartidas.h"
#include "checkML.h"
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void apagar(const tListaPartidas &partidas, ofstream &archivo) {  // Guarda la lista de partidas en el flujo archivo y elimina la memoria empleada por cada partida
	archivo << partidas.contador - numeroPartidasTerminadas(partidas) << '\n';  //Guardamos en el archivo en primer lugar el contador y posteriormente cada partida mediante el recorrido indicado
	int ent, partidasTerminadas = 0;
	bool noHistorico = false;
	ofstream historico1;
	fstream historico2;
	ifstream auxiliar;	

	auxiliar.open("historico.txt");
	if (!auxiliar.is_open()) {
		noHistorico = true;
	}
	auxiliar.close();

	historico1.open("historico.txt", ios::app);
	if(noHistorico) historico1 << setw(11) << 0 << '\n';

	for (int pos = 0; pos < partidas.contador; pos++) {		
		if (partidas.partida[pos]->estadoPartida != Terminada) {
			guardar(partidas.partida[pos], archivo);
		}
		else {
			guardar(partidas.partida[pos], historico1);
			partidasTerminadas++;
		}		
		delete partidas.partida[pos];
	}
	historico1.close();
	if (partidasTerminadas != 0) {
		historico2.open("historico.txt", ios::in | ios::out);
		historico2.seekg(0, ios::beg);
		historico2 >> ent;
		historico2.seekp(0, ios::beg);
		historico2 << setw(11) << ent + partidasTerminadas;
		historico2.close();
	}
	delete[] partidas.partida;
}

int numeroPartidasTerminadas(const tListaPartidas &partidas) { //Dada una lista de partidas, devuelve la cantidad de partidas terminadas que hay en ella.
	int terminadas = 0;
	for (int i = 0; i < partidas.contador; i++) {
		if (partidas.partida[i]->estadoPartida == Terminada) {
			terminadas++;
		}
	}
	return terminadas;
}

bool cargar(tListaPartidas &partidas, ifstream &archivo) {  //Carga la lista de partidas desde el flujo archivo
	bool ok = true;
	int pos = 0;
	
	while (ok && pos < partidas.contador){		
		partidas.partida[pos] = new tPart;
		ok = cargar(partidas.partida[pos], archivo);
		pos++;
	}

	return ok;
}

bool insertar(tListaPartidas &partidas, const tPartida &partida) {  // Dada una partida, si hay espacio en la lista, la añade al final de esta
	bool ok;
	if (partidas.contador < partidas.dimensionPartidas) { //En caso de que haya espacio disponible para insertar una partida la insertamos
		ok = true;
		partidas.partida[partidas.contador] = partida;
		partidas.contador++;
	}
	else ok = false;

	return ok;
}