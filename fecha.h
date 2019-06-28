// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#ifndef fecha_h //Realizamos un ifndef para evitar que fecha en alg�n m�dulo aparezca definida dos veces, 
				//ya que en el m�dulo gestor y mainP5 nos encontrar�amos con problemas
#define fecha_h
#include <iostream>


typedef time_t tFecha; //Definimos el tipo tFecha

tFecha fechaActual();//Funci�n que nos permite saber cu�l es la fecha actual

std::string stringFecha(tFecha fecha, bool hora); //Funci�n que nos permite pasar un tFecha a un formato v�lido de d�a/mes/a�o (DD/MM/AA) con hora opcional

#endif fecha_h