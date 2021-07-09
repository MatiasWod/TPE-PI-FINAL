#include "fileADT.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define BLOQUE 10
#define MAX_LINE 250
#define FATAL_ERROR 3
#define NOT_YEAR 4
#define NOT_TITLETYPE 5
#define ADDED 0
#define TOKENIZE(token,c) (token = strtok(NULL, c))


typedef struct line{
    char titleType;         //1 si es pelicula 2 si es serie y 0 si no es ninguna de las dos.
    char * primaryTitle;    //El nombre de la pelicula/serie.
    unsigned int startYear; //Si es una película, el año que salio. Si es una serie, en qué año comenzó a emitirse
    char ** genres;         //Una matriz para guardar los distintos generos.
    char* averageRating;    //Un número entre 0 y 10, con un decimal del rating de la pelicula/serie.
    unsigned int numVotes;  //Cantidad de votos que obtuvo la pelicula/serie.
}LineCDT;

//Prototipos de funciones static
static int whatTitleType(char*s1);
static int allocString(char **target,char* source);
static char * getString(char*s,char c,unsigned int* pos);

/*
Creo un nuevo adt. Devuelve un puntero al struct y NULL si no pudo
alocar memoria para el adt.
*/
LineADT newLine(void)
{
    return calloc(1,sizeof(LineCDT));
}


/*
Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si tiene una linea siguiente y 0 si se llego
al final del archivo.
*/
int hasNextLine(FILE*file)
{
    return feof(file) == 0;
}

/*
Retorna 1 si es una pelicula, retorna 2 si es una tvSeries
y 0 en cualquier otro caso.
*/
static int whatTitleType(char*s1)
{
    if(strcmp(s1,"movie")== 0)
        return MOVIE;
    else if(strcmp(s1,"tvSeries")==0)
        return TV_SERIES;
    return NOT_TITLETYPE;
}

/*
Aloco memoria al heap para guardar un string y copio en target
lo que tiene source. Retorno 0 si se copio correctamente y 3 sino.
*/
static int allocString(char **target,char* source)
{
    unsigned int i=0;
    while(source[i] != '\0'){
        if(i % BLOQUE == 0){
            *target = realloc(*target,(i+BLOQUE)*sizeof(char));
            if(*target == NULL)
                return FATAL_ERROR;
        }
        (*target)[i] = source[i];
        i++;
    }
    *target = realloc(*target,(i+1)*sizeof(char));
    if(*target == NULL)
        return FATAL_ERROR;
    (*target)[i] = '\0';
    return ADDED;
}

/*
Devuelvo un string(alocado en el heap) que tiene copiado el string "s"
a partir de la posicion "pos" hasta la primera aparicion del caracter c
o NULL. Ademas, en "pos" dejo hasta donde recorrio el string "s".
*/
static char * getString(char*s,char c,unsigned int* pos)
{
    unsigned int i=*pos,k=0;
    char * rta = malloc((k+BLOQUE)*sizeof(char));
    if(rta == NULL)
        return NULL;
    while(s[i] != c && s[i] != ';'){
        if(k % BLOQUE == 0 && k!= 0){
            rta = realloc(rta,(k+BLOQUE)*sizeof(char));
            if(rta == NULL)
                return NULL;
        }
        rta[k++] = s[i++];
    }
    rta = realloc(rta,(k+1)*sizeof(char));
    rta[k] = '\0';
    if (s[i] == ';')
        *pos = i-1;
    else 
        *pos = i;
    return rta;
}

/*
Recorro la linea del file y lo voy almacenando en el ADT. Retorna 3 si 
no hay memoria, retorna 1 si la linea no es una serie ni una pelicula y
retorna 0 si se se añadio correctamente al ADT. 
*/
int nextLine(LineADT line,FILE*file)
{
    int c = fgetc(file);
    if (!hasNextLine(file)){
        ungetc(c, file);
        return FATAL_ERROR;
    }
    ungetc(c, file);
    
    /*Hacemos un string para guardar toda la linea del file*/
    char *lineFile = malloc(MAX_LINE*sizeof(char));
    if(lineFile == NULL)
        return FATAL_ERROR;
    fgets(lineFile,MAX_LINE,file);
    lineFile =strtok(lineFile,";");

    //titleType
    int rta = whatTitleType(lineFile);
    if(rta == NOT_TITLETYPE){
        line->titleType = rta;
        free(lineFile);
        return NOT_TITLETYPE;
    }    
    line->titleType = rta;
    
    /*
    Voy recorriendo y analizando los distintos
    campos de la linea del file, unicamente analizando
    los que son importantes para los queries.
    */
    
    //PrimaryTitle
    char *token;
    TOKENIZE(token,";");
    int ok = allocString(&line->primaryTitle,token);
    if(ok == FATAL_ERROR)
        return FATAL_ERROR;
    
    //startYear
    TOKENIZE(token,";");
    if(token == NULL){
        free(lineFile);
        return NOT_YEAR;
    }
    if (!isdigit(token[0])){
        free(lineFile);
        return NOT_YEAR;
    }
    line->startYear = atoi(token);
      
    //endYear
    TOKENIZE(token,";");
    
    //genres
    TOKENIZE(token,";");
    if(line->titleType == MOVIE){
        int i = 0;
        unsigned int pos=0;
        line->genres = malloc(BLOQUE*sizeof(char*));
        while(token[pos] != ';'){
            char *subToken = getString(token,',',&pos);
            pos+=1;
            line->genres[i] = subToken;
            i++;
            if (i % BLOQUE == 0){
                line->genres = realloc(line->genres, (i+BLOQUE)*sizeof(char*));
            }
        }
        line->genres[i] = NULL;
    }

    //averageRating
    TOKENIZE(token,";");
    ok = allocString(&line->averageRating,token);
    if(ok == FATAL_ERROR)
        return FATAL_ERROR;

    //numVotes
    TOKENIZE(token,";");
    line->numVotes = atoi(token);

    //Libero el string donde se almaceno la linea donde estoy parado en el file
    free(lineFile);
    return ADDED;
}

/*
Devuelve el titleType del line en el que esta parado.
*/
char getTitleType(LineADT line)
{
    return line->titleType;
}

/*
Devuelve una copia del string del primaryTitle
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente.
*/
char *getPrimaryTitle(LineADT line)
{
    char *rta = NULL;
    int ok = allocString(&rta,line->primaryTitle);
    if(ok == ADDED)
        return rta;
    else
        return NULL;
}

/*
Devuelve el startYear (en unsigned int) de la linea del file.
*/
unsigned int getStartYear(LineADT line)
{
    return line->startYear;
}

/*
Devuelve la matriz de generos de la linea del file.
*/
char ** getGenres(LineADT line){
    return line->genres;
}

/*
Devuelve un string del averageRating de la linea del file.
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente.
*/
char * getAverageRating(LineADT line)
{
    char *rta = malloc(sizeof(char));
    int ok = allocString(&rta,line->averageRating);
    if(ok == ADDED)
        return rta;
    return NULL;
}

/*
Devuelve el numVotes(en unsigned int) de la linea del file.
*/
unsigned int getNumVotes(LineADT line)
{
    return line->numVotes;
}

/*
Libera los campos del struct que tienen alocada memoria en el heap
y les asigna NULL a cada puntero para la siguiente iteracion del LineADT.
*/
void freeLine(LineADT line) 
{
    if(line->primaryTitle != NULL){
        free(line->primaryTitle);
        line->primaryTitle = NULL;
    }
    if(line->genres != NULL){
        int i=0;
        while (line->genres[i] != NULL)
            free(line->genres[i++]);
        free(line->genres);
        line->genres = NULL;
    }
    if(line->averageRating != NULL){
        free(line->averageRating);
        line->averageRating = NULL;
    }
}

/*
Libero el struct, sin liberar los campos.
*/
void freeLineADT(LineADT line)
{
    free(line);
}