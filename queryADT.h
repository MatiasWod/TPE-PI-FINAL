#ifndef __query__h
#define __query__h
#include "fileADT.h"
#include <stdlib.h>
typedef struct queryCDT *queryADT;

// static int compare(unsigned int year1,unsigned int year2){
//         return ((int)year1-(int)year2);
// }

queryADT newQuery(void);//Crea la query vacia

void printList(queryADT query);

unsigned int add(queryADT query,LineADT data);//Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar y 0 si no pudo.

unsigned int hasNext(queryADT query);//Verifica si hay un siguiente elemento

void toBegin(queryADT query);//Resetea el iterador

void nextYear(queryADT query);//Avanza el iterador al siguiente anio

char * getFilmsNSeries(queryADT query); //Devuelve la cantidad de peliculas y series en un anio

char * getGenre(queryADT query);//todos los generos de un anio y cantidad de peliculas de cada genero, el vector tiene la cantidad de peliculas para cada genero

char * getMostVoted(queryADT query);//devuelve en string la pelicula y la serie mas votada del current year en el siguiente formato: startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie

void freeQuery(queryADT query);//Libera las querys

#endif