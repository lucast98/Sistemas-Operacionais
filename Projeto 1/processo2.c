#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

/** System calls usadas: fork(), cria o processo que é identico ao atual
                         waitpid(pid, &statloc, opts) faz o processo pai esperar o que processo filho esta processando
                         execve(name, argv, envp) antes o filho é igual ao pai, com isso ele substitui a funcionalidade antiga */
int main() {
    int pid, status, i=0; // pid = identificador de processos
    char *args[] = {"/bin/ls", NULL};

    while(i < 100){
        pid = fork(); /** cria um processo filho */ 
        if(pid < 0){ /** condicao de erro */
            printf("Erro no fork");
            return 0;
        }
        if(pid != 0){ 
            printf("Criado o processo %d\n", pid);
            waitpid(-1, &status, 0); /** processo pai espera o filho */
            printf("Processo pai espera o filho (%d) agir\n", pid);
        }else{
            if(execve("/bin/ls", args, NULL) == -1){ /** filho executa sua nova funcionalidade, retorna um valor e printa os arquivos do diretorio atual, comando ls */
                printf("Erro");
            }
        }
        i++;
    }
    return 0;
}
