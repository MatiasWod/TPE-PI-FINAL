#include "func.h"
#include "fileADT.h"
#include "queryADT.h"

int main(int argc, char ** argv)
{
    FILE * stream = fopen("imdbv3.csv", "rt");                   // Inicializo puntero a FILE
    unsigned int errors = checkArgs(argc, argv, stream); 
    if (errors == 1){ 
		printf("WRONG_ARG_COUNT: Debe recibir un solo argumento.");
		return 1;
    } else if (errors == 2) {
		printf("FILE_NOT_FOUND: El archivo no existe.");
		return 2;
    }

    LineADT linea = newLine();   // Recibo puntero a struct en el cual se depositaran los datos de la linea del csv
    queryADT list = newQuery();         // Creo estructura que tendra un puntero al primer nodo de una lista con los anios y sus respectivas peliculas
    unsigned int ok;
    // Recorro archivo csv de entrada
    while (hasNextLine(stream)){         // Mientras el archivo csv de entrada tenga una linea siguiente
        nextLine(linea, stream);       // Deposito en el struct apuntado por "linea" los datos de la primera linea del archivo csv de entrada
        ok = add(list, linea);        // Lleno con pelicula de la linea actual en el nodo de anio correspondiente. Si el nodo no existe lo agrego.
        if (ok == 0){
          printf("NO_MEM: No hay suficiente memoria en el heap."); // Si la variable de retorno del nextLine es 0, significa que no hay memoria suficiente para alocar en heap
          return 3;
        }
        freeLine(linea);                 // Libero los elementos del struct linea para depositar los datos de la siguiente linea del archivo csv
    }
    // Creo archivos en los cuales guardar las respuestas de los query 1, 2 y 3
    FILE * queryOne = fopen("query1.csv", "w+t");
    FILE * queryTwo = fopen("query2.csv", "w+t");
    FILE * queryThree = fopen("query3.csv", "w+t");

    // Seteo list en el primer nodo (anio mas chico) para recorrer
    toBegin(list);
    while (hasNext(list)){              // Mientras haya un anio siguiente
        char * qOne = getFilmsNSeries(list);         // Agrego al query1.csv
        fputs(qOne, queryOne);
        fputc('\n', queryOne);
        char * qTwo = getGenre(list);          // Agrego al query2.csv
        fputs(qTwo, queryTwo);
        fputc('\n', queryOne);
        char * qThree = getMostVoted(list);   // Agrego al query3.csv
        fputs(qThree, queryThree);
        fputc('\n', queryOne);
        nextYear(list);                // Avanzo al proximo anio
    }

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
