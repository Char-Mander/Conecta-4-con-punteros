#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

//Este módulo sirve para que la información (sobre la posible basura que ha podido dejar
//el programa al terminar) muestre el nombre del módulo y la línea de código.