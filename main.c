#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));

    printf("%s --> %d", content, leitura);

    int fd2 = open(ficheiro2, O_WRONLY|O_APPEND);

    if (fd2 == -1){
        perror("Erro ao abrir");
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

int main(int argc, char *argv[]){

    if(strcmp(argv[1], "cat") == 0) mostrar(argv[2]);
    if(strcmp(argv[1], "copy") == 0) copiar(argv[2]);
    if(strcmp(argv[1], "append") == 0) concatenar(argv[2], argv[3]);
    if(strcmp(argv[1], "count") == 0) contar(argv[2]);

    return 1;
}