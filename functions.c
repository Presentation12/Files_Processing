#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
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
    char content[1500];

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
 * TODO: Por alterar Permissoes, ficheiro2 tem igual a ficheiro_copia
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
    
    //Alterar permissoes
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
 
    int fd = open (ficheiro, O_RDONLY);
    char content[1500];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    struct stat fileStat;
    if(stat(ficheiro,&fileStat) < 0)    
        return 1;
 
    printf("Information for %s\n",ficheiro);
    printf("---------------------------\n");
    printf("File Size: %d\t",fileStat.st_size);
    printf("Blocks: %d\t", fileStat.st_blocks);
    printf("File inode: %d\t",fileStat.st_ino);
    printf("Links: %d\n",fileStat.st_nlink);
    printf("Acess: (");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(")");
    printf("\n\n");
 
    printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
 
    return 1;
}

/**
 * @brief Função com funcionalidade de apresentar uma lista de todas as pastas e ficheiros existentes na diretoria inserida
 * 
 * TODO: Melhorar mensagens de erro
 * 
 * @param diretoria Diretoria alvo 
 * @return int 
 */
int lista(char* diretoria){
    DIR* directory = opendir(diretoria);
    struct dirent *dir;

    if(directory == NULL){
        if(errno == ENOENT) write(1, "Diretorio nao existe", 33);
        else write(1, "Nao consegue abrir o diretorio", 40);
        printf("\n");
    }
    
    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                if(dir->d_type == DT_REG) printf("%s (file)\n", dir->d_name); 
                else if(dir->d_type == DT_DIR) printf("%s (directory)\n", dir->d_name); 
                else if(dir->d_type == DT_BLK) printf("%s (block device)\n", dir->d_name); 
                else if(dir->d_type == DT_CHR) printf("%s (character device)\n", dir->d_name); 
                else if(dir->d_type == DT_LNK) printf("%s (symbolic link)\n", dir->d_name); 
                else if(dir->d_type == DT_SOCK) printf("%s (local-domain socket)\n", dir->d_name); 
                else if(dir->d_type == DT_FIFO) printf("%s (fifo)\n", dir->d_name); 
                else if(dir->d_type == DT_UNKNOWN) printf("%s (unknown)\n", dir->d_name);   
            } 
        }
    }

    closedir(directory);

    return 1;
}