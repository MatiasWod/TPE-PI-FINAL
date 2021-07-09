
#include "fileADT.h"
#include "queryADT.h"
#include "defines.h"

/*Prototipos de funciones*/
FILE * genOutputFile(char * filename, char * mode, char * formatoQuery);


int main(int argc, char ** argv)
{
    FILE * stream = fopen(argv[1], "rt");
    if (argc != 2){
      fprintf(stderr, "WRONG_ARG_COUNT: Debe recibir un solo argumento.\n");             // Solo debe haber un argumento (sumado al nombre del archivo a ejecutar)
	    return WRONG_ARG_COUNT;                                                            // Si hay mas de un argumento devuelvo codigo de error 1
    }                                                                                    // "Abro" archivo si existe, si no existe devuelve NULL
    if (stream == NULL){
		  fprintf(stderr, "FILE_NOT_FOUND: El archivo no existe.\n");                        // Si no existe el archivo devuelvo codigo de error 2
      return FILE_NOT_FOUND;
    }

    /*
    LLamo las funciones para  crear los TADs y verifico si fueron alocados en el heap
    */
    LineADT linea = newLine();
    if(linea == NULL){
      fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
      return FATAL_ERROR;
    }
    queryADT list = newQuery();
    if(list == NULL){
      fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
      return FATAL_ERROR;
    }

    /*
    Recorro archivo csv de entrada depositando la informacion de la linea del file
    en el TAD del fileADT y pasandosela a la funcion add del queryADT para guardarlo como
    lista. Mientras no haya ningun error de alocacion de memoria
    ni tome algo que sea distinto de los titleType que queremos.
    */
    unsigned int ok, flag = 0;
    while (flag != FATAL_ERROR && flag != REACHED_EOF){
        flag = nextLine(linea, stream);
        if (flag == FATAL_ERROR){
          freeQuery(list);
          freeLine(linea);
          freeLineADT(linea);
          fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
          return FATAL_ERROR;
        }
        if (flag == OK){
          ok = add(list, linea);
          if (ok == FATAL_ERROR){
            freeLine(linea);
            freeLineADT(linea);
            freeQuery(list);
            fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
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

    char * qOne;
    char * qTwo;
    char * qThree;

    toBegin(list);
    while (hasNext(list)){
        qOne = getFilmsNSeries(list);
        if (qOne == NULL){
          freeLineADT(linea);
          freeQuery(list);
          fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
          return FATAL_ERROR;
        }
        fputs(qOne, queryOne);
        fputc('\n', queryOne);
        free(qOne);
        qTwo = getGenre(list);
        if (qTwo == NULL){
          freeLineADT(linea);
          freeQuery(list);
          fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
          return FATAL_ERROR;
        }else{
          fputs(qTwo, queryTwo);
          fputc('\n', queryTwo);
          free(qTwo);
        } 
        qThree = getMostVoted(list);
        if (qThree == NULL){
          freeLineADT(linea);
          freeQuery(list);
          fprintf(stderr, "FATAL_ERROR: No hay suficiente memoria en el heap.\n");
          return FATAL_ERROR;
        }
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
    printf("Ha finalizado la ejecucion con exito.\n");
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