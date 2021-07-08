#include "fileADT.h"
#include <stdlib.h>
#include <string.h>

#define BLOQUE 10
#define MAX_DIGIT_YEAR 4
#define MAX_RAITING_DIGIT 5
#define MAX_LINE 234
#define NO_MEM 0
#define OK 1
#define ADDED 2
#define MOVIE 1
#define TV_SERIES 2
#define TOKENIZE(token,c) (token = strtok(NULL, c))

typedef struct node{
    char *genre;
    struct node * tail;
}TNode;

typedef struct line{
    char titleType; //1 si es pelicula 2 si es serie.
    char * primaryTitle; //el nombre de la pelicula/serie
    unsigned int startYear;// si es una película, el año. Si es una serie, en qué año comenzó a emitirse
    TList firstGenre; //Hago una lista para guardar los generos.
    char* averageRating; //un número entre 0 y 10, con un decimal(ver de dejarlo en un decimal)
    unsigned int numVotes;// cantidad de votos que obtuvo
}LineCDT;

//Prototipos de funciones static
static int whatTitleType(char*s1);
static int allocString(char **target,char* source);
static TList addRec(TList list,char*s,int * ok);
static void freeRec(TList list);
static char * getString(char*s,char c,unsigned int* pos);

/*
Creo un nuevo adt. Devuelve un puntero al struct y NULL si no pudo
alocar memoria para el adt
*/
LineADT newLine(void)
{
    return calloc(1,sizeof(LineCDT));
}


/*
Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si tiene una linea siguiente y 0 si se llego
al final del archivo
*/
int hasNextLine(LineADT line,FILE*file)
{
    return feof(file) != 0;
}

/*
Retorna 1 si es una pelicula, retorna 2 si es una tvSeries
 y 0 para cualquier otra cosa
 */
static int whatTitleType(char*s1)
{
    if(strcmp(s1,"movie")== 0)
        return MOVIE;
    else if(strcmp(s1,"tvSeries")==0)
        return TV_SERIES;
    return 0;
}

/*
Aloco memoria al heap para guardar un string y copio en target
lo que tiene source. Retorno 1 si se copio correctamente y 0 sino.
*/
static int allocString(char **target,char* source)
{
    unsigned int i=0;
    while(source[i] != '\0'){
        if(i % BLOQUE == 0){
            *target = realloc(*target,(i+BLOQUE)*sizeof(char));
            if(*target == NULL)
                return NO_MEM;
        }
        (*target)[i] = source[i];
        i++;
    }
    *target = realloc(*target,(i+1)*sizeof(char));
    (*target)[i] = '\0';
    return OK;
}

/*
Agrego recursivamente un nodo que representa un genero a la lista
de generos
*/
static TList addRec(TList list,char*s,int * ok)
{
    if(list == NULL ||strcmp(list->genre,s) < 0){
        TList new = malloc(sizeof(TNode));
        new->genre = s;//Le paso el puntero del string que esta en el heap
        *ok=OK;
        new->tail = list;
        return new;
    }
    //else c < 0, entonces sigo recorriendo la lista
    //No considero c==0 ya que estaria mal el formato del .csv
    list->tail = addRec(list->tail,s,ok);
    return list;
}

/*
Devuelvo un string(alocado en el heap) que tiene copiado el string s
a partir pos hasta la primera aparicion del caracter c o null. Ademas,
en pos dejo hasta donde recorrio
*/
static char * getString(char*s,char c,unsigned int* pos)
{
    unsigned int i=*pos,k=0;//Con uno itero el string y el string rta respectivamente
    char * rta = malloc((k+BLOQUE)*sizeof(char));
    if(rta == NULL)
        return NULL;//Si no se pudo asignar memoria devuelvo NULL
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
Recorro la linea del file y lo voy almacenando en el ADT
Retorna 0 si no hay memoria,1 si la linea no es una serie ni una pelicula
Retorna 2 si se se añadio correctamente al ADT
*/
int nextLine(LineADT line,FILE*file)
{
    //Hacemos un string de toda la linea que vamos a liberar al final
    //de la funcion
    char *lineFile = malloc(MAX_LINE*sizeof(char));
    if(lineFile == NULL)
        return NO_MEM;
    fgets(lineFile,MAX_LINE,file);
    lineFile =strtok(lineFile,";");

    //titleType
    int rta = whatTitleType(lineFile);
    if(rta == 0)
        return 1;//Si no es ni pelicula ni serie,no me interesa almacenarlo
    line->titleType = rta;
    //Paso al siguiente campo de la linea
    //PrimaryTitle
    char *token;
    TOKENIZE(token,";");
    int ok = allocString(&line->primaryTitle,token);
    if(ok == NO_MEM)
        return NO_MEM;
    //startYear
    TOKENIZE(token,";");
    line->startYear = atoi(token);

      
    //endYear, ignoro el campo ya que no lo uso
    TOKENIZE(token,";");
    
    //genres
    //Si es serie, ignoro esta iteracion de token, si es pelicula
    //guardo los generos en una lista
    TOKENIZE(token,";");
    if(line->titleType == MOVIE){
        unsigned int pos=0;
        while(token[pos] != ';'){
            char *subToken = getString(token,',',&pos);
            pos+=1;
            line->firstGenre = addRec(line->firstGenre,subToken,&ok);
            if(ok == NO_MEM)
                return NO_MEM;
        }
    }
    
 
    //averageRating
    TOKENIZE(token,";");
    ok = allocString(&line->averageRating,token);
    if(ok == NO_MEM)
        return NO_MEM;

    //numVotes
    TOKENIZE(token,";");
    line->numVotes = atoi(token);

    //No recorro el resto de la linea ya que no es importante para estos
    //queries

    //Si llegue a esta instancia pude guardar sin problema en el tad
    //Libero el string donde se almaceno la linea donde estoy parado en el file
    free(lineFile);
    //Devuelvo added para indicar que se agrego correctamente
    return ADDED;
}

static void freeRec(TList list)
{
    if(list == NULL)
        return;
    freeRec(list->tail);
    free(list->genre);
    free(list);
}

/*
Libero la linea en la que estoy parado
*/
void freeLine(LineADT line)
{//Tengo que hacer un if a cada campo que se le pudo haber asignado memoria 
    if(line->primaryTitle != NULL)
        free(line->primaryTitle);
    if(line->firstGenre != NULL)
        freeRec(line->firstGenre);
    if(line->averageRating != NULL)
        free(line->averageRating);
    line->primaryTitle = NULL;//Lo seteo en NULL para la siguiente iteracion
    line->firstGenre = NULL;//de la linea del archivo
    line->averageRating = NULL;
}

/*
Libero todo el struct
*/
void freeLineADT(LineADT line)
{
    free(line);
}