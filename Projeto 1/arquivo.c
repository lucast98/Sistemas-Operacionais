#include<stdio.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/** System calls usadas: creat(nome, modo), open(arquivo, como, ...), write(fd, buffer, nbytes) e close(fd) */
int main() {
    int fd, s; // fd = diretorio do arquivo, s = retorno caso de erro
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // modos de operação do arquivo
    char *filename = "novo.txt"; // nome do arquivo a ser criado
    fd = creat(filename, mode); //cria novo arquivo, passando como paramentro o mode, caso bem sucedido retorna >= 0
    if(fd<0){
        return -1;
    }
    fd = open(filename, O_WRONLY | O_APPEND); //abre o arquivo
    if(fd<0){
        return -1;
    }
    write(fd, "texto", 5); //escreve "texto" no arquivo
    
    s = close(fd); // fecha o arquivo
    if(s == -1) //erro
        printf("Arquivo nao foi fechado corretamente!\n");
    else
        printf("Arquivo fechado corretamente!\n");
    
    return 0;
}