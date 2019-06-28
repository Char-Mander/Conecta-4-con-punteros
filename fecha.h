// LAURA JIMENEZ FERNANDEZ & DAVID LOSILLA CADENAS  
// L2-G12

#ifndef fecha_h //Realizamos un ifndef para evitar que fecha en algún módulo aparezca definida dos veces, 
				//ya que en el módulo gestor y mainP5 nos encontraríamos con problemas
#define fecha_h
#include <iostream>


typedef time_t tFecha; //Definimos el tipo tFecha

tFecha fechaActual();//Función que nos permite saber cuál es la fecha actual

std::string stringFecha(tFecha fecha, bool hora); //Función que nos permite pasar un tFecha a un formato válido de día/mes/año (DD/MM/AA) con hora opcional

#endif fecha_h