#include "queryADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define MOVIE 1
#define TV_SERIES 2

//Para almacenar la pelicula/serie con mas votos. Estructura especifica para el query3
typedef struct Q3{
    unsigned int maxVotesP;//La pelicula con mas rating (real con un decimal)
    char *maxRatingP;
    char *nameMaxP;//String con el nombre de la pelicula
    unsigned int maxVotesS;//La serie con mas rating (real con un decimal)
    char *maxRatingS;
    char *nameMaxS;//String con el nombre de la serie
}Q3;

//los generos tambien los vamos guardando en lista y agregamos en orden ascendente
typedef struct generos{
    struct generos *tail;
    char *nameGenero; 
    unsigned int cantGen;//cantidad de generos por pelicula
}Generos;

typedef Generos *TGeneros;
typedef Q3 * TQ3;

//se guarda por orden descendente
typedef struct year{
    unsigned int year;
    unsigned int cantPel;
    unsigned int cantSeries;
    TGeneros first;//el 1ero de la lista de generos de ese año
    TQ3 query3;
    struct year *tail;
}Year;

typedef Year *Tyear;

typedef struct queryCDT{
    Tyear startYear;
    Tyear currentYear;
};

//Pasa un unsigned int a string
static char *intAString(char *str,unsigned int num){
    int digitos=floor(log10(abs(num)))+1;
    str=malloc(sizeof(char)*digitos+1);
    sprintf(str,"%d",num);
    return str;
}
//Crea la query vacia
queryADT newQuery(void){
    return calloc(1,sizeof(struct queryCDT));
}


static TGeneros addGenRec(TGeneros first,TList new){
    int c;
    if (new==NULL)
        return first;
    if (first==NULL||(c=strcmp(new->genero,first->nameGenero))<0){
        TGeneros aux=calloc(1,sizeof(Generos));
        int ultimo;
        aux->nameGenero=copy(first->nameGenero,0,new->genero,&ultimo);//uso copy ya que me crea memoria y copia la string en una pasada
        aux->nameGenero[ultimo]='\0';
        aux->cantGen++;
        aux->tail=addGenRec(aux->tail,new->tail);
        return aux;
    }
    if (c==0)
        first=addGenRec(first,new->tail);
    else
        first->tail=addGenRec(first->tail,new);
}

static void addNewMax(char **maxRating,char **maxName,unsigned int *maxVotes,char *rating,char *name,unsigned int votes){
    int ultimo;
    *maxRating=copy(*maxRating,0,rating,&ultimo);            //uso copy ya que recorre solo una vez el string y crea exactamente la memoria que necesito
    (*maxRating)[ultimo]="\0";
    *maxName=copy(*maxName,0,name,&ultimo);
    (*maxName)[ultimo]='\0';
    *maxVotes=votes;
}

static Tyear addRec(Tyear year,LineADT data,int *ok){
    int c;
    if (year==NULL||(c=compare(data->startYear,year->year)>0)){
        Tyear aux=calloc(1,sizeof(Year));
        aux->year=data->startyear;
        if (data->titleType==MOVIE){
            aux->cantPel++;
            addNewMax(&aux->query3->maxRatingP,&aux->query3->nameMaxP,&aux->query3->maxVotesP,data->averageRating,data->primaryTitle,data->numVotes);
        }
        else{                       //No hace falta verificar que es TV_SERIES porque ya se hizo en el fileADT.c
            aux->cantSeries++;
            addNewMax(&aux->query3->maxRatingS,&aux->query3->nameMaxS,&aux->query3->maxVotesS,data->averageRating,data->primaryTitle,data->numVotes);
        }
        aux->first=addGenRec(aux->first,data->genres);
        aux->tail=year;
        *ok=1;
        return aux;
    }
    if (c==0){ //si estoy en el mismo anio que data
        if (data->titleType==MOVIE){
            year->cantPel++;
            if (year->query3->maxVotesP<data->numVotes)
                addNewMax(&year->query3->maxRatingP,&year->query3->nameMaxP,&year->query3->maxVotesP,data->averageRating,data->primaryTitle,data->numVotes);
        }
        else{
            year->cantSeries++;
            if (year->query3->maxVotesS<data->numVotes)
                addNewMax(&year->query3->maxRatingS,&year->query3->nameMaxS,&year->query3->maxVotesS,data->averageRating,data->primaryTitle,data->numVotes);
            }
            year->first=addGenRec(year->first,data->genres);
            *ok=1;
    }
    else
        year->tail=addRec(year->tail,data,ok);   
    return year;
    }

//Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar y 0 si no pudo.
unsigned int add(queryADT query,LineADT data){
    int ok=0;
    query->startYear=(query->startYear,data,&ok);
    return ok;
}

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
    char *str=intAString(str,query->currentYear->year);
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