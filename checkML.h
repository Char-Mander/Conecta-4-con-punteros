#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

//Este m�dulo sirve para que la informaci�n (sobre la posible basura que ha podido dejar
//el programa al terminar) muestre el nombre del m�dulo y la l�nea de c�digo.