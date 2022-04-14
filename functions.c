#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"


/**
 * @brief Função que escreve no stdout o conteúdo de um ficheiro
 * 
 * TODO: função completa
 * 
 * @param ficheiro -> ficheiro alvo
 * @return int 
 */
int mostrar(char* ficheiro){
    int fd = open (ficheiro, O_RDONLY), leitura;
    char* content = malloc(sizeof(1500));

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));
    
    write(1, content, leitura);
    printf("\n");

    close(fd);

    return 1;
}

/**
 * @brief Função que copia o conteúdo de um ficheiro para um novo ficheiro
 * 
 * TODO: Função Concluida
 * 
 * @param ficheiro -> ficheiro alvo
 * @return int 
 */
int copiar(char* ficheiro){
    char* ficheiro_copia = malloc(sizeof(ficheiro) + sizeof(".copia")); //criar file copia
    char* ficheiro2 = malloc(sizeof(ficheiro)); //guardar file numa variavel auxiliar (para nao perder o nome da mesma)
    strcpy(ficheiro2,ficheiro); //copiar nome de file para a auxiliar
    strcpy(ficheiro_copia,strcat(ficheiro2,".copia")); //montar o nome do novo file copia

    int fd = open (ficheiro, O_RDONLY), leitura;
    char content[1500];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));
    
    int fd2 = creat(ficheiro_copia, O_RDONLY|S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

    write(fd2, content, leitura);

    close(fd);
    close(fd2);

    return 1;
}

/**
 * @brief Função que concatena o ficheiro 1 no ficheiro 2
 * 
 * TODO: Função Concluida
 * 
 * @param ficheiro1 -> Ficheiro origem
 * @param ficheiro2 -> Ficheiro destino (concatenado)
 * @return int 
 */
int concatenar(char* ficheiro1, char* ficheiro2){
    int fd = open (ficheiro1, O_RDONLY), leitura;
    char content[1500];

    if (fd == -1) 
    {
        perror("Erro ao abrir (First Argument)");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));

    printf("%s --> %d", content, leitura);

    int fd2 = open(ficheiro2, O_WRONLY|O_APPEND);

    if (fd2 == -1){
        perror("Erro ao abrir (Second Argument)");
        return -1;
    }

    int test = write(fd2, content, leitura);

    close(fd2);
    close(fd);

    return 1;
}

/**
 * @brief Função que permite contar e mostrar ao user o número de linhas que um ficheiro contém
 * 
 * TODO: Função completa
 * 
 * @param ficheiro Ficheiro alvo
 * @return int 
 */
int contar(char* ficheiro){
    int counter = 1;
    int fd = open (ficheiro, O_RDONLY), leitura;
    char content[1500];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));

    for(int i = 0; i < sizeof(content); i++){
        if (content[i] == '\n') counter++;
    }

    printf("O ficheiro %s contem %d linhas\n", ficheiro, counter);

    return 1;
}

/**
 * @brief Função com funcionalidade de apagar um ficheiro com o nome passado por parametro
 * 
 * TODO: Função Concluida
 * 
 * @param ficheiro Ficheiro alvo
 * @return int 
 */
int apagar(char* ficheiro){
    if(remove(ficheiro) == -1)
    {
        perror("Erro ao abrir");
        return -1;
    }

    return 1;
}

/**
 * @brief Função com funcionalidade de mostrar a meta-informação do mesmo (simulação comando stat <file>)
 * 
 * TODO: Por começar...
 * 
 * @param ficheiro Ficheiro alvo
 * @return int 
 */
int informar(char* ficheiro){

    return 1;
}

/**
 * @brief Função com funcionalidade de apresentar uma lista de todas as pastas e ficheiros existentes na diretoria inserida
 * 
 * TODO: Por começar...
 * 
 * @param diretoria Diretoria alvo 
 * @return int 
 */
int lista(char* diretoria){

    return 1;
}