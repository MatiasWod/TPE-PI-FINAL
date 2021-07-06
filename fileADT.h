#include <stdio.h>

#ifndef __file__h
#define __file__h

//El file se recorrera una unica vez entonces no hace falta un iterador de lista
//ni nada ya que se va a liberar y guardar linea por linea a medida que se va 
//recorriendo el archivo

typedef LineCDT* LineADT;

//Creo un nuevo adt a partir de un puntero a file que se le pasa a la funcion.
LineADT newLine(FILE * file);

//LLamo a feof para ver si llegue al final del archivo
int hasNextLine(LineADT line,FILE*file);

//Recorro la linea del file y lo voy almacenando en el ADT
int nextLine(LineADT line,FILE*file);

//Libero la linea en la que estoy parado
void freeLine(LineADT line);

//Libero todo el struct
void freeLineADT(LineADT line);

#endif