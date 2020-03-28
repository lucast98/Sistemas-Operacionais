#include <stdio.h>

void main(){
    FILE *fp1, *fp2;
    char ch;
    int pos;

    if ((fp1 = fopen("lorem.txt","r")) == NULL){ //tenta abrir o arquivo   
        printf("\nFile cannot be opened");
        return;
    }
    else{
        printf("File opened for copy...\n");
    }
    fp2 = fopen("novo.txt", "w"); //abre/cria novo arquivo 
    fseek(fp1, 0L, SEEK_END); //ponteiro do arquivo no final
    pos = ftell(fp1);
    fseek(fp1, 0L, SEEK_SET); //ponteiro do arquivo no inicio
    while (pos--)
    {
        ch = fgetc(fp1);  //copia caractere por caractere do arquivo
        for(int i=0; i < 10000; i++)//faz 10000 copias de cada arquivo
            fputc(ch, fp2);
    }
    fclose(fp1); //fecha o primeiro arquivo
    fclose(fp2); //fecha o segundo arquivo
}