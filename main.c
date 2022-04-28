#include <string.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if(strcmp(argv[1], "cat") == 0) mostrar(argv[2]);
    if(strcmp(argv[1], "copy") == 0) copiar(argv[2]);
    if(strcmp(argv[1], "append") == 0) concatenar(argv[2], argv[3]);
    if(strcmp(argv[1], "count") == 0) contar(argv[2]);
    if(strcmp(argv[1], "rm") == 0) apagar(argv[2]);
    if(strcmp(argv[1], "ls") == 0) 
        if(argc == 2) lista(".");
        else lista(argv[2]);
    if(strcmp(argv[1], "stat") == 0) informar(argv[2]);

    return 1;
}
