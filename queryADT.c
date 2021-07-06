#include "queryADT.h"
#include <stdlib.h>

//Para almacenar la pelicula/serie con mas votos.
typedef struct Q3{
    unsigned int maxVotesP;//La pelicula con mas rating (real con un decimal)
    float maxRatingP;
    char *nameMaxP;//String con el nombre de la pelicula
    unsigned int maxVotesS;//La serie con mas rating (real con un decimal)
    float maxRatingS;
    char *nameMaxS;//String con el nombre de la serie
}TQ3;

//los generos tambien los vamos guardando en lista y agregamos en orden ascendente
typedef struct generos{
    struct generos *tail;
    char * nameGenero;
    unsigned int cantGen;//cantidad de generos por pelicula
}TGeneros;

typedef TYear * PYear;
typedef TGeneros *PGeneros;
typedef TQ3 * PQ3;

//se guarda por orden descendente
typedef struct queryCDT{
    struct año *tail;
    unsigned int startYear;
    unsigned int cantPel;
    unsigned int cantSeries;
    TGeneros *first;//el 1ero de la lista de generos de ese año
    TQ3 * query3;
};

queryADT newQuery(void);//Crea la query vacia

unsigned int add(queryADT query);//Agrega nueva pelicua/serie y devuelve 1 si se pudo agregar y 0 si no pudo.

void hasNext(queryADT query);//Pasa al siguiente elemento

void toBegin(queryADT query);//Resetea el iterador

char *getYear(queryADT query);//Devuelve el current year en string

char *getFilms(queryADT query);//Numero total de cantidad de peliculas de un anio, devuelto en string

char *getSeries(queryADT query);//Numero total de cantidad de series de un anio, devuelto en string

char **getGenre(queryADT query,unsigned int *cantFilms);//todos los generos de un anio y cantidad de peliculas de cada genero, el vector tiene la cantidad de peliculas para cada genero

char *getMostVoted(queryADT query);//devuelve en string la pelicula y la serie mas votada del current year en el siguiente formato: startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie

void nextYear(queryADT query);//Pasa al siguiente anio

void freeQuery(queryADT query);//Libera las querys