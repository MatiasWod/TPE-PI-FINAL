#include <stdio.h>

unsigned int checkArgs(int argc, char ** argv, FILE * archivo);

void AddQ1Line(char * year, char * films, char * series, FILE * stream);

void AddQ2Line(char * year, char ** genres, char ** films,FILE *stream);

void AddQ3Line(char * str,FILE *stream);