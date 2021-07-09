#ifndef __query__h
#define __query__h

#include "fileADT.h"
#include <stdlib.h>
#include "defines.h"

typedef struct queryCDT *queryADT;

/*
Crea un nuevo adt. Devuelve un puntero al struct y NULL si no pudo
alocar memoria para el adt.
*/
queryADT newQuery(void);

/*
Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar
y 0 si no pudo.
*/
unsigned int add(queryADT query,LineADT data);

/*
Verifica si hay un siguiente elemento en la lista
*/
unsigned int hasNext(queryADT query);

/*
Hace que el iterador apunte al principio de la lista de años
*/
void toBegin(queryADT query);

/*
Avanza el iterador al siguiente año
*/
void nextYear(queryADT query);

/*
Devuelve la cantidad de peliculas y series en un año en formato de string:
"year;films;series".
*/
char * getFilmsNSeries(queryADT query);

/*
Devuelve todos los generos de un año y cantidad de peliculas de cada genero en
formato de string: "year;genre;film"
*/
char * getGenre(queryADT query);

/*
Devuelve en string la pelicula y la serie mas votada del current year
en formato de string: 
"startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie".
*/
char * getMostVoted(queryADT query);

/*
Libera el adt y sus campos
*/
void freeQuery(queryADT query);

#endif