#include "queryADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define MOVIE 1
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
static Tyear addRec(Tyear year,LineADT data,int *ok);
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
        aux->nameGenero=copy(first->nameGenero,0,new->genre,&ultimo);//uso copy ya que me crea memoria y copia la string en una pasada
        if (first->nameGenero!=NULL)
            *ok=1;
        else{
            *ok=0;
            return NULL;
        }
        aux->nameGenero[ultimo]='\0';
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
    int ultimo;
    *maxRating=copy(*maxRating,0,rating,&ultimo);         //uso copy ya que recorre solo una vez el string y crea exactamente la memoria que necesito
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
            if (aux->query3->maxRatingP==0||aux->query3->nameMaxP==0)             //Me fijo si se quedo sin memoria
                return NULL;
        }
        else{                       //No hace falta verificar que es TV_SERIES porque ya se hizo en el fileADT.c
            aux->cantSeries++;
            addNewMax(&aux->query3->maxRatingS,&aux->query3->nameMaxS,&aux->query3->maxVotesS,data->averageRating,data->primaryTitle,data->numVotes);
            if (aux->query3->maxRatingS==0||aux->query3->nameMaxS==0)
                return NULL;
        }
        aux->first=addGenRec(aux->first,data->genres,ok);
        aux->tail=year;
        return aux;
    }
    if (c==0){ //si estoy en el mismo anio que data
        if (data->titleType==MOVIE){
            year->cantPel++;
            if (year->query3->maxVotesP<data->numVotes){
                addNewMax(&year->query3->maxRatingP,&year->query3->nameMaxP,&year->query3->maxVotesP,data->averageRating,data->primaryTitle,data->numVotes);
                if (year->query3->maxRatingP==0||year->query3->nameMaxP==0)
                    return NULL;
            }
        }
        else{
            year->cantSeries++;
            if (year->query3->maxVotesS<data->numVotes){
                addNewMax(&year->query3->maxRatingS,&year->query3->nameMaxS,&year->query3->maxVotesS,data->averageRating,data->primaryTitle,data->numVotes);
                if (year->query3->maxRatingS==0||year->query3->nameMaxS==0)
                    return NULL;
            }
            year->first=addGenRec(year->first,data->genres,ok);
    }
    }
    else
        year->tail=addRec(year->tail,data,ok);   
    return year;
    }

//Agrega nueva pelicua/serie con sus datos y devuelve 1 si se pudo agregar y 0 si no pudo.
unsigned int add(queryADT query,LineADT data){
    int ok=0;
    query->startYear=addRec(query->startYear,data,&ok);
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