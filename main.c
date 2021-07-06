#include "func.h"
#include "fileADT.h"
#include "queryADT.h"

#define BLOCK 50

int main(int argc, char ** argv)
{
    FILE * stream;
    unsigned int errors == checkArgs(argc, argv, stream); 
    if (errors == 1){
		printf(“WRONG_ARG_COUNT: Debe recibir un solo argumento.”);
		return 1;
    } else if (errors == 2) {
		printf(“FILE_NOT_FOUND: El archivo no existe.”);
		return 2;
    }
    LineADT linea = newLine(stream);
    queryADT list = newQuery();
    while (hasNextLine(linea)){
        list = add(list);
        linea = nextLine(stream);
        freeLine(linea);
    }
    FILE * queryOne = fopen("query1.csv", "w+t");
    FILE * queryTwo = fopen("query2.csv", "w+t");
    FILE * queryThree = fopen("query3.csv", "w+t");
    int * cantFilms;
    char * year;
    list = toBegin(list);
    while (hasNext(list)){
        year = getYear(list);
        AddQ1Line(year, getFilms(list), getSeries(list), stream);
        AddQ2Line(year, getGenre(list, cantFilms), cantFilms, stream);
        AddQ3Line(getMostVoted(list), stream);
        list = nextYear(list);
    }
}
