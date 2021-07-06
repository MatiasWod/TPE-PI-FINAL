#include "func.h"
#include "fileADT.h"
#include "queryADT.h"

int main(int argc, char ** argv)
{
    FILE * stream;                      // Inicializo puntero a FILE
    unsigned int errors == checkArgs(argc, argv, stream); 
    if (errors == 1){ 
		printf("WRONG_ARG_COUNT: Debe recibir un solo argumento.");
		return 1;
    } else if (errors == 2) {
		printf("FILE_NOT_FOUND: El archivo no existe.");
		return 2;
    }

    LineaADT linea = newLine(stream);   // Recibo puntero a struct en el cual se depositaran los datos de la linea del csv
    queryADT list = newQuery();         // Creo estructura que tendra un puntero al primer nodo de una lista con los anios y sus respectivas peliculas

    // Recorro archivo csv de entrada
    while (hasNextLine(linea)){         // Mientras el archivo csv de entrada tenga una linea siguiente
        errors = nextLine(linea);       // Deposito en el struct apuntado por "linea" los datos de la primera linea del archivo csv de entrada
        if (errors == 0){               // Si la variable de retorno del nextLine es 0, significa que no hay memoria suficiente para alocar en heap
            printf("NO_MEM: No hay suficiente memoria en el heap.");
            return 3;
        }
        list = add(list, linea);        // Lleno con pelicula de la linea actual en el nodo de anio correspondiente. Si el nodo no existe lo agrego.
        freeLine(line);                 // Libero los elementos del struct linea para depositar los datos de la siguiente linea del archivo csv
    }
    // Creo archivos en los cuales guardar las respuestas de los query 1, 2 y 3
    FILE * queryOne = fopen("query1.csv", "w+t");
    FILE * queryTwo = fopen("query2.csv", "w+t");
    FILE * queryThree = fopen("query3.csv", "w+t");

    int * cantFilms;
    char * year;
    // Seteo list en el primer nodo (anio mas chico) para recorrer
    list = toBegin(list);
    while (hasNext(list)){              // Mientras haya un anio siguiente
        year = getYear(list);           // Creo variable con el anio del nodo actual
        AddQ1Line(year, getFilms(list), getSeries(list), stream);       // Agrego al query1.csv
        AddQ2Line(year, getGenre(list, cantFilms), cantFilms, stream);  // Agrego al query2.csv
        AddQ3Line(getMostVoted(list), stream);  // Agrego al query3.csv
        list = nextYear(list);                  // Avanzo al proximo anio
    }
    // Cierro archivos "abiertos"
    fclose(stream);
    fclose(queryOne);
    fclose(queryTwo);
    fclose(queryThree);
    // Seteo list en primer nodo para liberar memoria
    toBegin(list);
    freeQuery(list);
    freeStruct(line);

    return 0;
}
