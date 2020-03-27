#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>

/** System calls usadas: creat(nome, modo), open(arquivo, como, ...), write(fd, buffer, nbytes) e close(fd) */
int main() {
    int fd, s, i=0; // fd = diretorio do arquivo, s = retorno caso de erro
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
    while(i<10000){
    write(fd, "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.", 575); //escreve "texto" no arquivo
       i++; 
    }
    s = close(fd); // fecha o arquivo
    if(s == -1) //erro
        printf("Arquivo nao foi fechado corretamente!\n");
    else
        printf("Arquivo fechado corretamente!\n");
    return 0;
}