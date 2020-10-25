
#ifndef MAPEO_H_INCLUDED
#define MAPEO_H_INCLUDED
#include "lista.h"
#include "mapeo.h"

// Macros con c�digos de finalizaci�n
#define MAP_ERROR_MEMORIA           100

void crear_mapeo(tMapeo *m, int ci,int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    (* m)->longitud_tabla = capacidad; //?
    (* m)->cantidad_elementos = 0;
    (* m)->hash_code = fHash;
    (* m)->comparador = fComparacion;
    (*m)->tabla_hash = tablaHash;
}
