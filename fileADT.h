#ifndef __fileADT__h
#define __fileADT__h

#include <stdio.h>

#define MOVIE 1
#define TV_SERIES 2

typedef struct line* LineADT;

/*
Creo un nuevo adt. Devuelve un puntero al struct y NULL si no pudo
alocar memoria para el adt.
*/
LineADT newLine(void);

/*
Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si tiene una linea siguiente y 0 si se llego
al final del archivo.
*/
int hasNextLine(FILE*file);

/*
Recorro la linea del file y lo voy almacenando en el ADT. Retorna 3 si 
no hay memoria, retorna 1 si la linea no es una serie ni una pelicula y
retorna 0 si se se añadio correctamente al ADT. 
*/
int nextLine(LineADT line,FILE*file);

/*
Devuelve el titleType del line en el que esta parado.
*/
char getTitleType(LineADT line);

/*
Devuelve una copia del string del primaryTitle
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente.
*/
char *getPrimaryTitle(LineADT line);

/*
Devuelve el startYear (en unsigned int) de la linea del file.
*/
unsigned int getStartYear(LineADT line);

/*
Devuelve la matriz de generos de la linea del file.
*/
char **getGenres (LineADT line);

/*
Devuelve un string del averageRating de la linea del file.
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente.
*/
char * getAverageRating(LineADT line);

/*
Devuelve el numVotes(en unsigned int) de la linea del file.
*/
unsigned int getNumVotes(LineADT line);

/*
Libera los campos del struct que tienen alocada memoria en el heap
y les asigna NULL a cada puntero para la siguiente iteracion del LineADT.
*/
void freeLine(LineADT line);

/*
Libera el struct, sin liberar los campos.
*/
void freeLineADT(LineADT line);

#endif