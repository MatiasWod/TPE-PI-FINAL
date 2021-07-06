#ifndef __query__h
#define __query__h

typedef struct queryCDT *queryADT;

queryADT newQuery(void);//Crea las querys vacias

void hasNext(queryADT query);//Pasa al siguiente elemento

void toBegin(queryADT query);//Resetea el iterador

void nextYear(queryADT query);//Pasa al siguiente anio

queryADT query1(queryADT query);//Devuelve cantidad de peliculas y series por anio

queryADT query2(queryADT query);//Devuelve cantidad de peliculas por anio y por genero

queryADT query3(queryADT query);//Devuelve la pelicua y la serie con mas votos de cada anio

void freeQuery(queryADT query);//Libera las querys

#endif