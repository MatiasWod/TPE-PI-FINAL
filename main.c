#include "func.h"
#include "fileADT.h"
#include "queryADT.h"
#define FORMATO_QUERY_1 ("year;films;series")
#define FORMATO_QUERY_2 ("year;genre;films")
#define FORMATO_QUERY_3 ("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie")

int main(int argc, char ** argv)
{
    FILE * stream = fopen(argv[1], "rt");                   // Inicializo puntero a FILE
    unsigned int errors = checkArgs(argc, argv, stream); 
    if (errors == 1){ 
		printf("WRONG_ARG_COUNT: Debe recibir un solo argumento.\n");
		return 1;
    } else if (errors == 2) {
		printf("FILE_NOT_FOUND: El archivo no existe.\n");
		return 2;
    }

    LineADT linea = newLine();   // Recibo puntero a struct en el cual se depositaran los datos de la linea del csv
    queryADT list = newQuery();         // Creo estructura que tendra un puntero al primer nodo de una lista con los anios y sus respectivas peliculas
    unsigned int ok, flag = 0;
    // Recorro archivo csv de entrada
    while (flag != 3){         // Mientras el archivo csv de entrada tenga una linea siguiente
        flag = nextLine(linea, stream);       // Deposito en el struct apuntado por "linea" los datos de la primera linea del archivo csv de entrada
        if (flag != 3 && flag != 4){
          ok = add(list, linea);        // Lleno con pelicula de la linea actual en el nodo de anio correspondiente. Si el nodo no existe lo agrego.
          if (ok == 3){
            printf("NO_MEM: No hay suficiente memoria en el heap.\n"); // Si la variable de retorno del nextLine es 0, significa que no hay memoria suficiente para alocar en heap
            return 3;
          }
          freeLine(linea);
        }     // Libero los elementos del struct linea para depositar los datos de la siguiente linea del archivo csv
    }

    fprintf(stderr, "asd\n");
    printList(list);
    fprintf(stderr,"PASSNEXTLINE\n");
    // Creo archivos en los cuales guardar las respuestas de los query 1, 2 y 3
    FILE * queryOne = fopen("query1.txt", "w+t");
    fputs(FORMATO_QUERY_1, queryOne);
    fputc('\n', queryOne);
    FILE * queryTwo = fopen("query2.txt", "w+t");
    fputs(FORMATO_QUERY_2, queryTwo);
    fputc('\n', queryTwo);
    FILE * queryThree = fopen("query3.txt", "w+t");
    fputs(FORMATO_QUERY_3, queryThree);
    fputc('\n', queryThree);

    // Seteo list en el primer nodo (anio mas chico) para recorrer
    toBegin(list);
    fprintf(stderr,"PASS1\n");
    while (hasNext(list)){              // Mientras haya un anio siguiente
        fprintf(stderr,"PASS2\n");
        char * qOne = getFilmsNSeries(list);         // Agrego al query1.csv
        fprintf(stderr,"PASSFilmsNSeries\n");
        fputs(qOne, queryOne);
        fputc('\n', queryOne);
        free(qOne);
        char * qTwo = getGenre(list);          // Agrego al query2.csv
        fprintf(stderr,"PASSgetGenre\n");
        fputs(qTwo, queryTwo);
        if (qTwo[0] != '\0' )
          fputc('\n', queryTwo);
        fprintf(stderr,"\n");
        free(qTwo);
        char * qThree = getMostVoted(list);   // Agrego al query3.csv
        fprintf(stderr,"getMostVoted\n");
        fputs(qThree, queryThree);
        fputc('\n', queryThree);
        nextYear(list);                // Avanzo al proximo anio
        free(qThree);
    }
    fprintf(stderr,"PASS\n");
    // Cierro archivos "abiertos"
    fclose(stream);
    fclose(queryOne);
    fclose(queryTwo);
    fclose(queryThree);
    // Seteo list en primer nodo para liberar memoria
    toBegin(list);
    freeQuery(list);
    freeLineADT(linea);

    return 0;
}
