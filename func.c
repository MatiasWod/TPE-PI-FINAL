#include "func.h"

#define WRONG_ARG_COUNT 1
#define FILE_NOT_FOUND 2
#define OK 0

unsigned int checkArgs(int argc, char ** argv, FILE * archivo){
    if (argc != 2)                      // Solo debe haber un argumento (sumado al nombre del archivo a ejecutar)
	    return WRONG_ARG_COUNT;         // Si hay mas de un argumento devuelvo codigo de error 1
    archivo = fopen(argv[1], “rt”);     // "Abro" archivo si existe, si no existe devuelve NULL
    if (archivo == NULL)                // Si no existe el archivo devuelvo codigo de error 2
		return FILE_NOT_FOUND;
	return OK;
}

// AddQ1Line guarda en el archivo parametro lo requerido para el query1
void AddQ1Line(char * year, char * films, char * series, FILE * stream){
    fputs(year, stream);                
    fputc(';', stream);
    fputs(films, stream);
    fputc(';', stream);
    fputs(series, stream);
    fputc('\n', stream);
    return;
}

// AddQ2Line guarda en el archivo parametro lo requerido para el query2
void AddQ2Line(char * year, char ** genres, char ** films, stream){
    for (i=0 ;genres[i][0] != '\0'; i++){
        fputs(year, stream);
        fputc(';', stream);
        fputs(genres[i], stream);
        fputc(';', stream);
        fputs(films[i], stream);
        fputc('\n', stream);
    }
    return;
}

// AddQ3Line guarda en el archivo parametro lo requerido para el query3
void AddQ3Line(char * str, stream){
    fputs(str, stream);
    fputc('\n', stream);
    return;
}

