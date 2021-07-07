#include "queryADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Para almacenar la pelicula/serie con mas votos.
typedef struct Q3{
    unsigned int maxVotesP;//La pelicula con mas rating (real con un decimal)
    char *maxRatingP;
    char *nameMaxP;//String con el nombre de la pelicula
    unsigned int maxVotesS;//La serie con mas rating (real con un decimal)
    char *maxRatingS;
    char *nameMaxS;//String con el nombre de la serie
}PQ3;

//los generos tambien los vamos guardando en lista y agregamos en orden ascendente
typedef struct generos{
    struct generos *tail;
    char *nameGenero; 
    unsigned int cantGen;//cantidad de generos por pelicula
}PGeneros;

typedef PGeneros *TGeneros;
typedef PQ3 * TQ3;

//se guarda por orden descendente
typedef struct year{
    unsigned int year;
    unsigned int cantPel;
    unsigned int cantSeries;
    TGeneros first;//el 1ero de la lista de generos de ese año
    TQ3 query3;
    struct year *tail;
}Pyear;

typedef Pyear *Tyear;

typedef struct queryCDT{
    Tyear startYear;
    Tyear currentYear;
};

//Crea la query vacia
queryADT newQuery(void){
    return calloc(1,sizeof(struct queryCDT));
}

//Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar y 0 si no pudo.
unsigned int add(queryADT query,lineADT data);

//Verifica si hay un siguiente elemento
unsigned int hasNext(queryADT query){
    return (query->currentYear->tail!=NULL);
}

//Resetea el iterador
void toBegin(queryADT query){
    query->currentYear=query->startYear;
}

//Devuelve el current year en string
char *getYear(queryADT query){
    int digitos=floor(log10(abs(query->currentYear->year))+1;
    char *str=malloc(sizeof(char)*digitos);
    sprintf(str,"%d",query->currentYear->year);
    return str;
}

//Numero total de cantidad de peliculas de un anio, devuelto en string
char *getFilms(queryADT query);

//Numero total de cantidad de series de un anio, devuelto en string
char *getSeries(queryADT query);

//todos los generos de un anio y cantidad de peliculas de cada genero, el vector tiene la cantidad de peliculas para cada genero
char **getGenre(queryADT query,unsigned int *cantFilms);

//devuelve en string la pelicula y la serie mas votada del current year en el siguiente formato: startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie
char *getMostVoted(queryADT query);

//Pasa al siguiente anio
void nextYear(queryADT query){
    query->currentYear=query->currentYear->tail;
    return;
}

//Libera las querys
void freeQuery(queryADT query);