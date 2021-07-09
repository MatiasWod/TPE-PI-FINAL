#include "func.h"
#include "fileADT.h"
#include "queryADT.h"

/*Constantes simbolicas*/
#define FORMATO_QUERY_1 ("year;films;series")
#define FORMATO_QUERY_2 ("year;genre;films")
#define FORMATO_QUERY_3 ("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie")
#define FATAL_ERROR 3
#define OK 0
#define WRONG_ARG_COUNT 1
#define FILE_NOT_FOUND 2

/*Prototipos de funciones*/
FILE * genOutputFile(char * filename, char * mode, char * formatoQuery);


int main(int argc, char ** argv)
{
    FILE * stream = fopen(argv[1], "rt");
    unsigned int errors = checkArgs(argc, argv, stream); 
    if (errors == WRONG_ARG_COUNT){ 
		  printf("WRONG_ARG_COUNT: Debe recibir un solo argumento.\n");
		  return WRONG_ARG_COUNT;
    } 
    else if (errors == FILE_NOT_FOUND) {
		  printf("FILE_NOT_FOUND: El archivo no existe.\n");
		  return FILE_NOT_FOUND;
    }

    /*
    LLamo las funciones para  crear los TADs y verifico si fueron alocados en el heap
    */
    LineADT linea = newLine();
    if(linea == NULL){
      printf("FATAL_ERROR: No hay suficiente memoria en el heap.\n");
      return FATAL_ERROR;
    }
    queryADT list = newQuery();
    if(list == NULL){
      printf("FATAL_ERROR: No hay suficiente memoria en el heap.\n");
      return FATAL_ERROR;
    }

    /*
    Recorro archivo csv de entrada depositando la informacion de la linea del file
    en el TAD del fileADT y pasandosela a la funcion add del queryADT para guardarlo como
    lista. Mientras no haya ningun error de alocacion de memoria
    ni tome algo que sea distinto de los titleType que queremos.
    */
    unsigned int ok, flag = 0;
    while (flag != FATAL_ERROR){
        flag = nextLine(linea, stream);
        if (flag == OK){
          ok = add(list, linea);
          if (ok == FATAL_ERROR){
            printf("FATAL_ERROR: No hay suficiente memoria en el heap.\n");
            return FATAL_ERROR;
          }
        }
        freeLine(linea);
    }

    /*
    Creo archivos en los cuales se van a guardar las respuestas de los query 1, 2 y 3
    */
    FILE * queryOne = genOutputFile("query1.csv", "w+t", FORMATO_QUERY_1);
    FILE * queryTwo = genOutputFile("query2.csv", "w+t", FORMATO_QUERY_2);
    FILE * queryThree = genOutputFile("query3.csv", "w+t", FORMATO_QUERY_3);

    /*
    Seteo list para recorrer la lista de años e ir guardando los queries
    en los archivos de queries correspondientes. Libreando cada string despues
    de agregarlo a los archivos.
    */
    toBegin(list);
    while (hasNext(list)){
        char * qOne = getFilmsNSeries(list);
        fputs(qOne, queryOne);
        fputc('\n', queryOne);
        free(qOne);
        char * qTwo = getGenre(list);
        if (qTwo != NULL ){
          fputs(qTwo, queryTwo);
          fputc('\n', queryTwo);
          free(qTwo);
        }
        char * qThree = getMostVoted(list);
        fputs(qThree, queryThree);
        fputc('\n', queryThree);
        nextYear(list);
        free(qThree);
    }

    /*
    Cierro los archivos abiertos.
    */
    freeQuery(list);
    freeLineADT(linea);
    fclose(stream);
    fclose(queryOne);
    fclose(queryTwo);
    fclose(queryThree);
    return 0;
}

/*
Abre el nombre del archivo que se le pasa por parametro de la funcion.
Prepara
*/
FILE * genOutputFile(char * filename, char * mode, char * formatoQuery){
  FILE * query = fopen(filename, mode);
  fputs(formatoQuery, query);
  fputc('\n', query);
  return query;
}