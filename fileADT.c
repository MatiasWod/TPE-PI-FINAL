#include "fileADT.h"
#include <stdlib.h>

#define BLOQUE 10
#define MAX_DIGIT_YEAR 4
#define MAX_LINE 234
#define NO_MEM 0
#define OK 1
#define MOVIE 1
#define TV_SERIES 2
#define TOKENIZE(token,c) (token = strtok(NULL, c))


typedef struct line{
    char titleType; //1 si es pelicual 0 si es serie.
    char * primaryTitle; //el nombre de la pelicula/serie
    unsigned int startYear;// si es una película, el año. Si es una serie, en qué año comenzó a emitirse
    char **genres; //Lista de géneros separados por coma
    char* averageRating; //un número entre 0 y 10, con un decimal(ver de dejarlo en un decimal)
    unsigned int numVotes;// cantidad de votos que obtuvo
}LineCDT;



//Creo un nuevo adt a partir de un puntero a file que se le pasa a la funcion.
LineADT newLine(void)
{
    return calloc(1,sizeof(LineCDT));
}


/*Llamo a feof para ver si llegue al final del archivo.
Devuelve algo distinto de 0 si llegue al final del archivo y 0 sino*/
int hasNextLine(LineADT line,FILE*file)
{
    return feof(file);
}

//Retorna 1 si es una pelicula, retorna 2 si es una tvSeries y 0 para cualquier otra cosa
static int whatTitleType(char*s1)
{
    if(strcmp(s1,"movie")== 0)
        return MOVIE;
    else if(strcmp(s1,"tvSeries"))
        return TV_SERIES;
    return 0;
}

//Copio un string a otro string y lo asigno en el heap
static int allocString(char *source,char* target)
{
    unsigned int i=0;
    while(source[i] != '\0'){
        if(i%BLOQUE==0){
            realloc(target,(i+BLOQUE)*sizeof(char));
            if(target == NULL)
                return NO_MEM;
        }
        target[i] = source[i];
        i++;
    }
    target[i++] = '\0';
    realloc(target,(i)*sizeof(char));
    return OK;
}

static int allocGenres(char**m,char*s)
{
    unsigned int i=0,j=0,k=0;//Itero el string y las filas y columnas de la matriz
    char ok;
    while(s[k] != ";"){
        if(i%BLOQUE ==0){
            realloc(m,(i+BLOQUE)*sizeof(char*));
            if(m == NULL)
                return NO_MEM;
        }
        ok = allocString(s+k,m[i]);
        if(ok == NO_MEM)
            return NO_MEM;
        i++;
    }
    return OK;
}

//Recorro la linea del file y lo voy almacenando en el ADT
//Retorna 0 si no hay memoria,1 si la linea no es una serie
int nextLine(LineADT line,FILE*file)
{
    while(feof(file)==0){
        //Hacemos un string de toda la linea que vamos a liberar al final
        //de la funcion
        char *lineFile = malloc(MAX_LINE*sizeof(char));
        if(lineFile == NULL)
            return NO_MEM;
        fgets(lineFile,MAX_LINE,file);//Poner esto antes de asignar memoria asi, no asigno al pedo
        char * token = strtok(lineFile,";");

        //titleType
        int rta = whatTitleType(token);
        if(rta == 0)
            return 1;//Si no es ni pelicula ni serie,no me interesa almacenarlo

        //Paso al siguiente campo de la linea
        //PrimaryTitle
        TOKENIZE(token,";");
        int ok = allocString(token,line->primaryTitle);
        if(ok == NO_MEM)
            return NO_MEM;
        
        //startYear
        TOKENIZE(token,";");
        line->startYear = atoi(token);
        
        //endYear, ignoro el campo ya que no lo uso
        TOKENIZE(token,";");

        //genres
        TOKENIZE(token,";");
        ok = allocGenres(line->genres,token);
        if(ok == NO_MEM)
            return NO_MEM;
        
        //averageRating
        TOKENIZE(token,";");
        ok = allocString(token,line->averageRating);
        if(ok == NO_MEM)
            return NO_MEM;

        //numVotes
        TOKENIZE(token,";");
        line->numVotes = atoi(token);


    }
}

/*
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
        char num[MAX_DIGIT_YEAR+1];
        num[MAX_DIGIT_YEAR] = '\0';//Lo hago null terminated
        i=0;
        while((c = fgetc(file)) !=';')
            num[i++] = c;
        line->startYear = atoi(num);

        //Ignoro endYear, no se usa para las queries dadas
        while((c=fgetc(file)) != ';');

        //Almaceno los generos en vector de strings
        i=0;
        int j=0,k=0;//itero filas y columnas de la matriz de genres
        while((c=fgetc(file)) != ';'){
            if(j%BLOQUE == 0)
                line->genres = malloc((j+BLOQUE)*sizeof(char*));
            while((c != ',')){
                if(k%BLOQUE == 0)
                    line->genres[j] = malloc((k+BLOQUE)*sizeof(char));
                line->genres[j][k++] = c;
                c = fgetc(file);
            }
            j++;
        }
*/

//Libero la linea en la que estoy parado
void freeLine(LineADT line);

//Libero todo el struct
void freeLineADT(LineADT line);