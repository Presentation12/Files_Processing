#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include "functions.h"

/**
 * @brief Função que escreve no stdout o conteúdo de um ficheiro
 * 
 * TODO: DONE
 * 
 * @param ficheiro -> ficheiro alvo
 * @return int 
 */
int mostrar(char* ficheiro){
    int fd = open (ficheiro, O_RDONLY), leitura;
    char content[50000];

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
 * TODO: DONE
 * 
 * @param ficheiro -> ficheiro alvo
 * @return int 
 */
int copiar(char* ficheiro){
    char* ficheiro_copia = malloc(sizeof(ficheiro) + sizeof(".copia")); //criar file copia
    strcpy(ficheiro_copia,ficheiro); // copiar nome do file
    strcat(ficheiro_copia,".copia"); // adicionar '.copia'

    int fd = open (ficheiro, O_RDONLY), leitura;
    char content[50000];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));
    
    //Alterar permissoes
    int fd2 = creat(ficheiro_copia, S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP|S_IROTH);

    write(fd2, content, leitura);

    close(fd);
    close(fd2);

    return 1;
}

/**
 * @brief Função que concatena o ficheiro 1 no ficheiro 2
 * 
 * TODO: DONE
 * 
 * @param ficheiro1 -> Ficheiro origem
 * @param ficheiro2 -> Ficheiro destino (concatenado)
 * @return int 
 */
int concatenar(char* ficheiro1, char* ficheiro2){
    int fd = open (ficheiro1, O_RDONLY), leitura;
    char content[50000];

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
 * TODO: DONE
 * 
 * @param ficheiro Ficheiro alvo
 * @return int 
 */
int contar(char* ficheiro){
    int counter = 1;
    int fd = open (ficheiro, O_RDONLY), leitura;
    char content[50000];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    leitura = read(fd, content, sizeof(content));

    for(int i = 0; i < leitura; i++){
        if (content[i] == '\n') counter++;
    }

    printf("O ficheiro %s contem %d linhas\n", ficheiro, counter);

    return 1;
}

/**
 * @brief Função com funcionalidade de apagar um ficheiro com o nome passado por parametro
 * 
 * TODO: DONE
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
 * TODO: DONE
 * 
 * @param ficheiro Ficheiro alvo
 * @return int 
 */
int informar(char* ficheiro){
 
    int fd = open (ficheiro, O_RDONLY);
    char content[50000];

    if (fd == -1) 
    {
        perror("Erro ao abrir");
        return -1;
    }

    struct stat info;

    if(stat(ficheiro,&info) < 0)    
        return 1;

    if(ficheiro[0] == '.') {
        printf("Erro ao abrir: No such file or directory\n");
        return 1;
    }

    struct passwd *user;
    user = getpwuid(info.st_uid);

    printf("File: %s\tType: ",ficheiro);

    switch (info.st_mode & S_IFMT) {
        case S_IFBLK:  
            printf("block device\n");
            break;
        case S_IFCHR:
            printf("character device\n");        
            break;
        case S_IFDIR:
            printf("directory\n");               
            break;
        case S_IFIFO:
            printf("FIFO/pipe\n");               
            break;
        case S_IFLNK: 
            printf("symlink\n");                 
            break;
        case S_IFREG:
            printf("regular file\n");            
            break;
        case S_IFSOCK:
            printf("socket\n");                  
            break;
        default:       
            printf("unknown\n");                
            break;
    }

    printf("File inode: %d\tUid: (%d/%s)\n",info.st_ino, info.st_uid,user->pw_name);
    printf("Access: %s", ctime(&info.st_atim));
    printf("Modify: %s", ctime(&info.st_mtim));
    printf("Change: %s", ctime(&info.st_ctim));
    printf("\n");
 
    return 1;
}

/**
 * @brief Função com funcionalidade de apresentar uma lista de todas as pastas e ficheiros existentes na diretoria inserida
 * 
 * TODO: DONE
 * 
 * @param diretoria Diretoria alvo 
 * @return int 
 */
int lista(char* diretoria){
    DIR* directory = opendir(diretoria);
    struct dirent *dir;

    if(directory == NULL){
        if(errno == ENOENT) write(1, "Diretorio nao existe", 25);
        else write(1, "Nao consegue abrir o diretorio", 40);
        printf("\n");
    }
    
    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && dir->d_name[0] != '.'){
                if(dir->d_type == DT_BLK){
                    printf("\033[0;33m");
                    printf("%s (block device)\n", dir->d_name); 
                    printf("\033[0m");
                } 
                else if(dir->d_type == DT_CHR){
                    printf("\033[40m");
                    printf("%s (character device)\n", dir->d_name);
                    printf("\033[0m");
                }  
                else if(dir->d_type == DT_DIR){
                    printf("\033[0;34m");
                    printf("%s (directory)\n", dir->d_name);
                    printf("\033[0m");
                }  
                else if(dir->d_type == DT_FIFO){
                    printf("\033[0;31m");
                    printf("%s (FIFO/pipe)\n", dir->d_name); 
                    printf("\033[0m");
                } 
                else if(dir->d_type == DT_LNK){
                    printf("\033[0;32m");
                    printf("%s (symlink)\n", dir->d_name);
                    printf("\033[0m");
                } 
                else if(dir->d_type == DT_REG){
                    printf("\033[0;37m");
                    printf("%s (regular file)\n", dir->d_name); 
                    printf("\033[0m");
                }
                else if(dir->d_type == DT_SOCK){
                    printf("\033[0;35m");
                    printf("%s (socket)\n", dir->d_name); 
                    printf("\033[0m");
                } 
                else printf("%s (unknown)\n", dir->d_name);   
            } 
        }
    }

    closedir(directory);

    return 1;
}