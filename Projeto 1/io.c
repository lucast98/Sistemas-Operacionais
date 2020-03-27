#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>

/** System calls usadas: cfsetospeed(&termios, speed) muda a velocidade de saida,
                         cfgetospeed(&termios, speed) pega a velocidade de saida, 
                         tcsetattr(fd, opt, &termios) ajusta os atributos da velocidade, 
                         tcgetattr(fd, &termios) pega os atributos*/

/* Função que auxilia o print da velocidade certa, pois sempre vai acontecer algum dos valores abaixo */
char *see_speed(speed_t speed) {
    static char   SPEED[20];
    switch (speed) {
        case B0:
            strcpy(SPEED, "B0");
            break;
        case B50:
            strcpy(SPEED, "B50");
            break;
        case B75:
            strcpy(SPEED, "B75");
            break;
        case B110:
            strcpy(SPEED, "B110");
            break;
        case B134:
            strcpy(SPEED, "B134");
            break;i
        case B150:
            strcpy( SPEED, "B150");
            break;
        case B200:
            strcpy(SPEED, "B200");
            break;
        case B300:
            strcpy(SPEED, "B300");
            break;
        case B600:
            strcpy(SPEED, "B600");
            break;
        case B1200:
            strcpy(SPEED, "B1200");
            break;
        case B1800:
            strcpy(SPEED, "B1800");
            break;
        case B2400:
            strcpy(SPEED, "B2400");
            break;
        case B4800:
            strcpy(SPEED, "B4800");
            break;
        case B9600:
            strcpy(SPEED, "B9600");
            break;
        case B19200:
            strcpy(SPEED, "B19200");
            break;
        case B38400:
            strcpy(SPEED, "B38400");
            break;
        default:
            sprintf(SPEED, "unknown (%d)", (int)speed);
  }
  return SPEED;
}

int main() {
    speed_t speed; // taxa de velocidade de transferencia 
    struct termios termAttr; // conjunto de instruções para interface de io do terminal 

    tcgetattr(STDOUT_FILENO, &termAttr); /** obtém os atributos de saida*/
    speed = cfgetospeed(&termAttr); /** obtem a velocidade de saida do que foi feito em termAttr */
    printf("Velocidade de saida: %s\n", see_speed(speed));
    if (speed != B9600){
        cfsetospeed(&termAttr, B9600); /** ajusta a velocidade de saida para B9600, pode ser qualquer um estamos apenas utilizando a funcao,  se ja nao for */
        tcsetattr(STDOUT_FILENO, TCSADRAIN, &termAttr); /** ajusta os atributos de saida */
        speed = cfgetospeed(&termAttr); /** obtem a nova velocidade de saida do que foi feito em termAttr */
        printf("Nova velocidade de saida: %s\n", see_speed(speed));
    }

    return 0;
}
