#include "queryADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define MOVIE 1
#define TV_SERIES 2
#define BLOCK 10

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

struct queryCDT{
    Tyear startYear;
    Tyear currentYear;
}queryCDT;

static char * copy(char * str, int pos, char * source, int * newPos );
static char *intAString(unsigned int num);
static TGeneros addGenRec(TGeneros first,TList new, int *ok);
static void addNewMax(char **maxRating,char **maxName,unsigned int *maxVotes,char *rating,char *name,unsigned int votes);
static Tyear addRec(Tyear year,char tipo,char *primaryTitle,unsigned int startYear,TList new,char *averageRating,unsigned int numVotes,int *ok);
static char *getYear(queryADT query);
static char * getFilms(queryADT query);
static char * getSeries(queryADT query);
static void freeRecGenero(TGeneros first);
static void freeRec(Tyear year);

//reasigna memoria a str y copia source desde la posicion pos y devuelve donde termina el string en newPos
static char * copy(char * str, int pos, char * source, int * newPos ){
    int i = pos,j;
    if (source == NULL) return NULL;
    str = realloc(str, (i+BLOCK));
    if (str == NULL)  return NULL;
    for (j = 0; source[j] != '\0'; j++, i++){
        if (i % BLOCK == 0){
            str = realloc(str, (i+BLOCK));
            if (str == NULL) return NULL;
        }
        str[i] = source[j];
    }
    str = realloc(str, i+1);
    if (str == NULL) return NULL;
    *newPos = i;
    return str;
}
//Pasa un unsigned int a string
static char *intAString(unsigned int num){
    int digitos=floor(log10(abs(num)))+1;
    char * str=malloc(sizeof(char)*digitos+1);
    if (str == NULL) return NULL;
    sprintf(str,"%d",num);
    return str;
}
//Crea la query vacia
queryADT newQuery(void){
    return calloc(1,sizeof(struct queryCDT));
}


static TGeneros addGenRec(TGeneros first,TList new, int *ok){
    int c;
    if (new==NULL)
        return first;
    if (first==NULL||(c=strcmp(new->genre,first->nameGenero))<0){
        TGeneros aux=calloc(1,sizeof(Generos));
        int ultimo;
        aux->nameGenero=new->genre;
        aux->cantGen++;
        aux->tail=addGenRec(aux->tail,new->tail,ok);
        return aux;
    }
    if (c==0)
        first=addGenRec(first,new->tail,ok);
    else
        first->tail=addGenRec(first->tail,new,ok);
    return first;
}

static void addNewMax(char **maxRating,char **maxName,unsigned int *maxVotes,char *rating,char *name,unsigned int votes){
    *maxRating=rating;
    *maxName=name;
    *maxVotes=votes;
}

static Tyear addRec(Tyear year,char tipo,char *primaryTitle,unsigned int startYear,TList new,char *averageRating,unsigned int numVotes,int *ok){
    int c;

    if (year==NULL||(c=compare(startYear,year->year)>0)){
        Tyear aux=malloc(sizeof(Year));
        if (aux==NULL)
            return NULL;
        aux->year=startYear; 
        if (tipo==MOVIE){
            aux->cantPel=1;
            aux->cantSeries=0;
            addNewMax(&aux->query3->maxRatingP,&aux->query3->nameMaxP,&aux->query3->maxVotesP,averageRating,primaryTitle,numVotes);
        }
        else if (tipo==TV_SERIES){                  
            aux->cantSeries=1;
            aux->cantPel=0;
            addNewMax(&aux->query3->maxRatingS,&aux->query3->nameMaxS,&aux->query3->maxVotesS,averageRating,primaryTitle,numVotes);
        }
        else{
            return year;   //Si no es MOVIE ni TV_SERIES retorno como estaba
        }
        aux->first=addGenRec(aux->first,new,ok);
        aux->tail=year;
        return aux;
    }
    else if (c==0){ //si estoy en el mismo anio que data
        if (tipo==MOVIE){
            year->cantPel++;
            if (year->query3->maxVotesP<numVotes)
                addNewMax(&year->query3->maxRatingP,&year->query3->nameMaxP,&year->query3->maxVotesP,averageRating,primaryTitle,numVotes);
            }
        else if (tipo==TV_SERIES){
            year->cantSeries++;
            if (year->query3->maxVotesS<numVotes)
                addNewMax(&year->query3->maxRatingS,&year->query3->nameMaxS,&year->query3->maxVotesS,averageRating,primaryTitle,numVotes);
        }
        else
            return year;
        year->first=addGenRec(year->first,new,ok);
    }
    else
        year->tail=addRec(year->tail,tipo,primaryTitle,startYear,new,averageRating,numVotes,ok);   
    return year;
    }

//Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar y 0 si no pudo.
unsigned int add(queryADT query,LineADT data){
    int ok=0;
    char tipo=getTitleType(data);
    char *primaryTitle=getPrimaryTitle(data);
    unsigned int startYear=getStartYear(data);
    TList new=getFirstGenre(data);
    char *averageRating=getAverageRating(data);
    unsigned int numVotes=getNumVotes(data);
    query->startYear=addRec(query->startYear,tipo,primaryTitle,startYear,new,averageRating,numVotes,&ok);
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
static char *getYear(queryADT query){
    return intAString(query->currentYear->year);
}

//Numero total de cantidad de peliculas de un anio, devuelto en string
static char * getFilms(queryADT query){
    return intAString(query->currentYear->cantPel);
}

//Numero total de cantidad de series de un anio, devuelto en string
static char * getSeries(queryADT query){
    return intAString(query->currentYear->cantSeries);
}

char * getFilmsNSeries(queryADT query){
    int i;
    char * str = NULL;
    str = copy(str, 0, getYear(query), &i);
    if (str == NULL)
        return NULL;
    str[i]=';';
    str = copy(str, i+1, getFilms(query), &i );
    if (str == NULL)
        return NULL;
    str[i]=';';
    str = copy(str, i+1, getSeries(query), &i);
    if (str == NULL)
        return NULL;
    str[i]='\0';
    return str;
}

//todos los generos de un anio y cantidad de peliculas de cada genero, el vector tiene la cantidad de peliculas para cada genero
char *getGenre(queryADT query){
    TGeneros iter = query->currentYear->first;
    int i = 0;
    char * genres = NULL;
    char * s = NULL;
    while (iter != NULL){
        genres = copy(genres, i, getYear(query), &i);
        if (genres == NULL)
            return NULL;
        genres[i]=';';
        genres = copy(genres, i+1, iter->nameGenero, &i);
        if (genres == NULL) 
            return NULL;
        genres[i]=';';
        genres = copy(genres, i+1,( s = intAString(iter->cantGen)), &i);
        if (genres == NULL) 
            return NULL;
        genres[i] = '\n';
        iter = iter->tail;
        i++;
        free(s);
    }
    genres[i-1] = '\0';
    return genres;
}

//devuelve en string la pelicula y la serie mas votada del current year en el siguiente formato: startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie
char *getMostVoted(queryADT query){
    int i = 0;
    char * res = NULL;
    char * year;
    char * votesM;
    char * votesS;
    res = copy(res, i, (year = intAString(query->currentYear->year)), &i);
    if (res == NULL)
        return NULL;
    res[i] = ';';
    res = copy(res, i+1, query->currentYear->query3->nameMaxP, &i);
    if (res == NULL) 
        return NULL;
    res[i] = ';';
    res = copy(res, i+1, (votesM = intAString(query->currentYear->query3->maxVotesP)), &i);
    if (res == NULL) 
        return NULL;
    res[i] = ';';
    res = copy(res, i+1, query->currentYear->query3->maxRatingP, &i);
    if (res == NULL) 
        return NULL;
    res[i]=';';
    res = copy(res, i+1, query->currentYear->query3->nameMaxS, &i);
    if (res == NULL) 
        return NULL;
    res[i]=';';
    res = copy(res, i+1, (votesS = intAString(query->currentYear->query3->maxVotesS)), &i);
    if (res == NULL) 
        return NULL;
    res[i]=';';
    res = copy(res, i+1, query->currentYear->query3->maxRatingP, &i);
    if (res == NULL) 
        return NULL;
    res[i]='\0';
    free(year);
    free(votesM);
    free(votesS);
    return res;
}

//Pasa al siguiente anio
void nextYear(queryADT query){
    query->currentYear=query->currentYear->tail;
    return;
}

static void freeRecGenero(TGeneros first){
        if (first!=NULL){
            freeRecGenero(first->tail);
            free(first);
        }
}

static void freeRec(Tyear year){
    if (year!=NULL){
        freeRec(year->tail);
        freeRecGenero(year->first);
        free(year->query3);
        free(year);
    }
}

//Libera las querys
void freeQuery(queryADT query){
    freeRec(query->startYear);
    free(query);
}