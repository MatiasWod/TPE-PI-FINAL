#include "queryADT.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define BLOCK 10

//Para almacenar la pelicula/serie con mas votos. Estructura especifica para el query3
typedef struct Q3{
    unsigned int maxVotesP;     //La pelicula con mas rating (real con un decimal)
    char *maxRatingP;
    char *nameMaxP;             //String con el nombre de la pelicula
    unsigned int maxVotesS;     //La serie con mas rating (real con un decimal)
    char *maxRatingS;
    char *nameMaxS;             //String con el nombre de la serie
}Q3;
typedef Q3 * TQ3;
//Los generos tambien los vamos guardando en lista y agregamos en orden ascendente
typedef struct generos{
    struct generos *tail;
    char *nameGenero; 
    unsigned int cantGen;//cantidad de generos por pelicula
}Generos;
typedef Generos *TGeneros;
//Se guarda por orden descendente
typedef struct year{
    unsigned int year;
    unsigned int cantPel;
    unsigned int cantSeries;
    TGeneros first;//el 1ero de la lista de generos de ese año
    TQ3 query3;
    struct year *tail;
}Year;
typedef Year *Tyear;
//El primer año(primer nodo de la lista) y el iterador de años(lista)
struct queryCDT{
    Tyear startYear;
    Tyear currentYear;
}queryCDT;

/*Prototipos de funciones static*/
static char * copy(char * str, int pos, char * source, int * newPos );
static char *intAString(unsigned int num);
static Tyear addRec(Tyear year,char tipo,char *primaryTitle,unsigned int startYear,char **new,char *averageRating,unsigned int numVotes,int *ok);
static void addNewMax(char **maxRating,char **maxName,unsigned int *maxVotes,char *rating,char *name,unsigned int votes, int *ok);
static char *getYear(queryADT query);
static char * getFilms(queryADT query);
static char * getSeries(queryADT query);
static void freeRecGenero(TGeneros first);
static void freeRec(Tyear year);


/*
Reasigna memoria a str y copia source desde la posicion pos
y devuelve donde termina el string en newPos.
*/
static char * copy(char * str, int pos, char * source, int * newPos ){
    int i = pos,j;
    if (source == NULL) 
        return str;
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

/*
Pasa un unsigned int a string
*/
static char *intAString(unsigned int num){
    unsigned int digitos;
    if (num == 0)
        digitos = 2;
    else digitos = floor(log10(num))+1;
    char * str=malloc(digitos+1);
    if (str == NULL)
        return NULL;
    sprintf(str,"%d",num);
    return str;
}

/*
Crea un nuevo adt. Devuelve un puntero al struct y NULL si no pudo
alocar memoria para el adt.
*/
queryADT newQuery(void){
    return calloc(1,sizeof(struct queryCDT));
}

/*
Agrega recursivamente generos en orden alfabetico a la lista de generos
*/
static TGeneros addGenRec(TGeneros first,char *new, int *ok){
    int c;
    if (new==NULL)
        return first;
    if (first==NULL||(c=strcmp(new,first->nameGenero))<0){
        TGeneros aux=calloc(1,sizeof(Generos));
        if (aux!=NULL)
            *ok =FATAL_ERROR;
        else{
            *ok=0;
            return first;
        }
        int i;
        aux->nameGenero=copy(aux->nameGenero,0, new, &i );
        aux->nameGenero[i]='\0';
        aux->cantGen++;
        aux->tail=first;
        return aux;
    }
    if (c==0){
        first->cantGen++;
    }
    else{
        first->tail=addGenRec(first->tail,new,ok);}
    return first;
}

/*
Agrega el nuevo maximo al struct, reemplazandolo  por el anterior
*/
static void addNewMax(char **maxRating,char **maxName,unsigned int *maxVotes,char *rating,char *name,unsigned int votes,int *ok){
    int ultimo;
    *maxRating = copy(*maxRating,0,rating, &ultimo );
    if (*maxRating==NULL){
        *ok=FATAL_ERROR;
        return;
    }
    (*maxRating)[ultimo]='\0';
    *maxName = copy(*maxName,0,name, &ultimo );
    if (*maxName==NULL){
        *ok=FATAL_ERROR;
        return;
    }
    (*maxName)[ultimo]='\0';
    *maxVotes = votes;
    *ok=0;
    return;
}

/*
Agrega recursivamente un año (nodo) a la lista de años en orden descendiente, llama addGenRec
y tambien llama a addNewMax si es que encuentra un nuevo maximo
*/
static Tyear addRec(Tyear year,char tipo,char *primaryTitle,unsigned int startYear,char **new,char *averageRating,unsigned int numVotes,int *ok){
    int c;
    if (year==NULL||(c=(int)startYear-year->year )>0){
        Tyear aux=calloc(1, sizeof(Year));
        if (aux==NULL)
            return NULL;
        aux->query3=calloc(1, sizeof(struct Q3));
        if (aux->query3==NULL)
            return NULL;
        aux->year=startYear; 
        if (tipo==MOVIE){
            aux->cantPel=1;
            aux->cantSeries=0;
            addNewMax(&(aux->query3->maxRatingP),&(aux->query3->nameMaxP),&(aux->query3->maxVotesP),averageRating,primaryTitle,numVotes,ok);
            if (*ok==FATAL_ERROR)
                return year;
            for (int i=0;new[i]!=NULL;i++){
                aux->first=addGenRec(aux->first,new[i],ok);
            }
        }
        else{                  
            aux->cantSeries=1;
            aux->cantPel=0;
            addNewMax(&aux->query3->maxRatingS,&aux->query3->nameMaxS,&aux->query3->maxVotesS,averageRating,primaryTitle,numVotes,ok);
        }
        if (*ok==FATAL_ERROR)
            return year;
        aux->tail=year;
        return aux;
    }
    else if (c==0){ //si estoy en el mismo anio que data
        if (tipo==MOVIE){
            year->cantPel++;
            for (int i=0;new[i]!=NULL;i++)
                year->first=addGenRec(year->first,new[i],ok);
            if (*ok==FATAL_ERROR)
                return year;
            if (year->query3->maxVotesP<numVotes)
                addNewMax(&year->query3->maxRatingP,&year->query3->nameMaxP,&year->query3->maxVotesP,averageRating,primaryTitle,numVotes,ok);
            }
        else{
            year->cantSeries++;
            if (year->query3->maxVotesS<numVotes)
                addNewMax(&year->query3->maxRatingS,&year->query3->nameMaxS,&year->query3->maxVotesS,averageRating,primaryTitle,numVotes,ok);
        }
    }
    else if (c < 0)
        year->tail=addRec(year->tail,tipo,primaryTitle,startYear,new,averageRating,numVotes,ok); 
    return year;
}

/*
Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar
y 0 si no pudo.
*/
unsigned int add(queryADT query,LineADT data){
    int ok=0;
    char tipo=getTitleType(data);
    char *primaryTitle=getPrimaryTitle(data); 
    unsigned int startYear=getStartYear(data);
    char **new;
    if (tipo == MOVIE)
        new=getGenres(data);
    else 
        new = NULL;
    char *averageRating=getAverageRating(data);
    unsigned int numVotes=getNumVotes(data);
    query->startYear=addRec(query->startYear,tipo,primaryTitle,startYear,new,averageRating,numVotes,&ok);
    free(primaryTitle);
    free(averageRating);
    return ok;
}

/*
Verifica si hay un siguiente elemento en la lista
*/
unsigned int hasNext(queryADT query){
    return (query->currentYear!=NULL);
}

/*
Hace que el iterador apunte al principio de la lista de años
*/
void toBegin(queryADT query){
    query->currentYear=query->startYear;
}


/*
Devuelve el current year en string
*/
static char *getYear(queryADT query){
    return intAString(query->currentYear->year);
}

/*
Numero total de cantidad de peliculas de un anio, devuelto en string
*/
static char * getFilms(queryADT query){
    return intAString(query->currentYear->cantPel);
}

/*
Numero total de cantidad de series de un anio, devuelto en string
*/
static char * getSeries(queryADT query){
    return intAString(query->currentYear->cantSeries);
}

/*
Devuelve la cantidad de peliculas y series en un año en formato de string:
"year;films;series".
*/
char * getFilmsNSeries(queryADT query){
    int i;
    char * str = NULL;
    char * auxToFreeOne;
    char * auxToFreeTwo;
    char * auxToFreeThree;
    str = copy(str, 0, (auxToFreeOne = getYear(query)), &i);
    if (str == NULL)
        return NULL;
    str[i]=';';
    str = copy(str, i+1, (auxToFreeTwo = getFilms(query)), &i );
    if (str == NULL)
        return NULL;
    str[i]=';';
    str = copy(str, i+1, (auxToFreeThree = getSeries(query)), &i);
    if (str == NULL)
        return NULL;
    str[i]='\0';
    free(auxToFreeOne);
    free(auxToFreeTwo);
    free(auxToFreeThree);
    return str;
}

/*
Devuelve todos los generos de un año y cantidad de peliculas de cada genero en
formato de string: "year;genre;film"
*/
char *getGenre(queryADT query){
    TGeneros iter = query->currentYear->first;
    if (iter == NULL){
        return NULL;
    }
    int i = 0;
    char * genres = NULL;
    char * s = NULL;
    char * aux = NULL;
    while (iter != NULL){
        genres = copy(genres, i, (aux =getYear(query)), &i);
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
        free(aux);
    }
    genres[i-1] = '\0';
    return genres;
}

/*
Devuelve en string la pelicula y la serie mas votada del current year
en formato de string: 
"startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie".
*/
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
    res = copy(res, i+1, query->currentYear->query3->maxRatingS, &i);
    if (res == NULL) 
        return NULL;
    res[i]='\0';
    free(year);
    free(votesM);
    free(votesS);
    return res;
}

/*
Avanza el iterador al siguiente año
*/
void nextYear(queryADT query){
    if (!hasNext(query))
        return;
    query->currentYear=query->currentYear->tail;
    return;
}

/*
Libera recursivamente la lista de generos.
*/
static void freeRecGenero(TGeneros first){
        if (first == NULL)
            return;
        freeRecGenero(first->tail);
        free(first->nameGenero);
        free(first);
}

/*
Libera recursivamente la lista de años.
*/
static void freeRec(Tyear year){
    if (year == NULL)
        return;
    freeRec(year->tail);
    freeRecGenero(year->first);
    free(year->query3->maxRatingP);
    free(year->query3->maxRatingS);
    free(year->query3->nameMaxP);
    free(year->query3->nameMaxS);
    free(year->query3);
    free(year);
}

/*
Libera el adt y sus campos
*/
void freeQuery(queryADT query){
    freeRec(query->startYear);
    free(query);
}