/*  
Software Básico - Trabalho 1
Isabela Maria Pereira Cruzeiro - 180102362
Sistema operacional: Windows 11
Editor: Visual Studio Code
Compilador: MinGW gcc 4.9.2
*/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include<ctype.h>
#include "def.h"

int main(int argc, char **argv){
    char modo[3];
    strcpy(modo, argv[1]); //passando para uma variavel, ficar mais legivel 
    if(strcmp(modo, "-p") == 0 || strcmp(modo, "-m") == 0 || strcmp(modo, "-o") == 0){
        roda(argv[2], argv[3], modo);
    }
    else{
        printf("Formato não suportado!\n");
    }
    return 0;
}