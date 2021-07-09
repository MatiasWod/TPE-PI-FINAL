#ifndef __fileADT__h
#define __fileADT__h
#include <stdio.h>

#define MOVIE 1
#define TV_SERIES 2
//El file se recorrera una unica vez entonces no hace falta un iterador de lista
//ni nada ya que se va a liberar y guardar linea por linea a medida que se va 
//recorriendo el archivo

typedef struct line* LineADT;

typedef struct node{
    char *genre;
    struct node * tail;
}TNode;

typedef struct node * TList;

/*---FUNCIONES A BORRAR---*/
//SON PARA TESTEO

// void printList(LineADT line);

void printListRec(TList list);

/*---ACA TERMINA EL DELIMITADOR DE FUNCIONES A BORRAR---*/

/*
Creo un nuevo adt a partir de un puntero a file que se le pasa a la funcion.
*/
LineADT newLine(void);

/*
Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si llegue al final del archivo y 0 sino
*/
int hasNextLine(FILE*file);

/*
Recorro la linea del file y lo voy almacenando en el ADT
Retorna 0 si no hay memoria,1 si la linea no es una serie y tampoco una pelicula
*/
int nextLine(LineADT line,FILE*file);

/*
Devuelve el titleType del line en el que esta parado
*/
char getTitleType(LineADT line);

/*
Devuelve una copia del string del primaryTitle
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente
*/
char *getPrimaryTitle(LineADT line);

/*
Devuelve el startYear (en unsigned int) de la linea del file
*/
unsigned int getStartYear(LineADT line);

/*
Devuelve una copia del primer nodo de la lista o NULL si no se pudo
alocar memoria correctamente
*/
// TList getFirstGenre (LineADT line);
char **getFirstGenre (LineADT line);
/*
Devuelve un string del averageRating de la linea del file
Retorna el puntero a la copia o NULL si no se pudo alocar memoria correctamente
*/
char * getAverageRating(LineADT line);

/*
Devuelve el numVotes(en unsigned int) de la linea del file
*/
unsigned int getNumVotes(LineADT line);

/*
Libero la linea en la que estoy parado
*/
void freeLine(LineADT line);

/*
Libero el struct
*/
void freeLineADT(LineADT line);

#endif