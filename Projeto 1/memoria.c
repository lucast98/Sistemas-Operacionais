#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>

/** System calls usadas: brk(addr), muda o tamanho do segmento de dados
                         mmap(addr, len, prot, flags, fd, offset) mapeia o arquivo na memoria
                         munmap(addr, len) remove o mapeamento */
int main() {
    size_t size = getpagesize(); /** tamanho da pagina */
    int s = brk(0); /** especifica o tamanho do segmento de dados, inicialmente com 0 */
    if(s == -1)
        return -1;
    char *map = mmap(0, size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_ANON|MAP_PRIVATE, 0, 0); /** mapeia um arquivo na memoria */
    if(map == MAP_FAILED)
        printf("Nao foi possivel mapear o arquivo na memoria\n");
    else{
        printf("Arquivo mapeado com sucesso!\n");
        printf("Agora tentaremos remover esse mapeamento...\n");
        int unmap = munmap(0, size); /** tenta remover o mapeamento do arquivo */
        if(unmap == -1)
            printf("O mapemento do arquivo nao foi removido\n");
        else
            printf("O mapemento do arquivo foi removido\n");
        
    }
    return 0;
}