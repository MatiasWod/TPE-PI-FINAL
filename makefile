COMPILER=gcc
OUTPUT_FILE=imdb
FILES= main.c queryADT.c fileADT.c func.c
FLAGS= -lm -pedantic -std=c99 -Wall -fsanitize=address -g

all:
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)
clean:
	rm -r $(OUTPUT_FILE) *.o