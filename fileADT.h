#ifndef __fileADT__h
#define __fileADT__h
#include <stdio.h>
//El file se recorrera una unica vez entonces no hace falta un iterador de lista
//ni nada ya que se va a liberar y guardar linea por linea a medida que se va 
//recorriendo el archivo

typedef struct line* LineADT;

typedef struct node * TList;

/*Creo un nuevo adt a partir de un puntero a file que se le pasa a la funcion.*/
LineADT newLine(void);

/*Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si llegue al final del archivo y 0 sino*/
int hasNextLine(LineADT line,FILE*file);

/*Recorro la linea del file y lo voy almacenando en el ADT
Retorna 0 si no hay memoria,1 si la linea no es una serie y tampoco una pelicula*/
int nextLine(LineADT line,FILE*file);

/*Libero la linea en la que estoy parado*/
void freeLine(LineADT line);

/*Libero todo el struct*/
void freeLineADT(LineADT line);

#endif