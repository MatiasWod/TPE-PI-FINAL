#include "fileADT.h"
#include <stdlib.h>

#define BLOQUE 10
#define MAX_DIGIT_YEAR 4

typedef struct line{
  char titleType; //1 si es pelicual 0 si es serie.
  char * primaryTitle; //el nombre de la pelicula/serie
  unsigned int startYear;// si es una película, el año. Si es una serie, en qué año comenzó a emitirse
  char **genres; //Lista de géneros separados por coma
  float averageRating; //un número entre 0 y 10, con un decimal(ver de dejarlo en un decimal)
  unsigned int numVotes;// cantidad de votos que obtuvo
}LineCDT;



//Creo un nuevo adt a partir de un puntero a file que se le pasa a la funcion.
LineADT newLine(FILE * file)
{
    return LineADT line = malloc(sizeof(LineCDT));
}

/*LLamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si llegue al final del archivo y 0 sino*/
int hasNextLine(LineADT line,FILE*file)
{
    return feof(file);
}

//Recorro la linea del file y lo voy almacenando en el ADT
int nextLine(LineADT line,FILE*file)
{
    while(feof(file)==0){
        char c = fgetc(file);
        
        //Veo si es una pelicula o una serie
        if(c == 'm')
            line->titleType=1;
        else
            line->titleType=0;
        
        //Avanzo hasta toparme con un ';' ignoro el resto del nombre de movie
        while((c = fgetc(file)) != ';');
        
        //Leo el primaryTitle
        int i=0;
        while((c = fgetc(file))!= ';'){
            if(i%BLOQUE ==0){
                line->primaryTitle = realloc(line->primaryTitle,(i+BLOQUE)*sizeof(char));
            }
            line->primaryTitle[i++] = c;
        }
        line->primaryTitle[i] = '\0';
        line->primaryTitle = realloc(line->primaryTitle,(i)*sizeof(char));
        
        //Paso el string del año a un int
        char num[MAX_DIGIT_YEAR];
        i=0;
        while((c = fgetc(file)) !=';')
            num[i++] = c;
        line->startYear = atoi(num);

        //Ignoro endYear, no se usa para las queries dadas
        while((c=fgetc(file)) != ';');

        //Almaceno los generos en vector de strings
        i=0;
        int j=0;
        while((c=fgetc(file)) != ';'){
            line->genres = malloc((j+BLOQUE)*sizeof(char*));
        }
    }
}

//Libero la linea en la que estoy parado
void freeLine(LineADT line);

//Libero todo el struct
void freeLineADT(LineADT line);