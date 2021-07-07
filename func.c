#include "func.h"

#define WRONG_ARG_COUNT 1
#define FILE_NOT_FOUND 2
#define OK 0

unsigned int checkArgs(int argc, char ** argv, FILE * archivo){
    if (argc != 2)                      // Solo debe haber un argumento (sumado al nombre del archivo a ejecutar)
	    return WRONG_ARG_COUNT;         // Si hay mas de un argumento devuelvo codigo de error 1
    archivo = fopen(argv[1], "rt");     // "Abro" archivo si existe, si no existe devuelve NULL
    if (archivo == NULL)                // Si no existe el archivo devuelvo codigo de error 2
		return FILE_NOT_FOUND;
	return OK;
}
