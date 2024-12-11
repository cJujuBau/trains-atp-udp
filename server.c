#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CHECK_ERROR(val1,val2,msg)   if (val1==val2) \
                                    { perror(msg); \
                                        exit(EXIT_FAILURE); }

#define LOCALPORT   3000
#define LOCALIP     "172.20.10.12"

#define MAXOCTETS   150

void init_server_socket(struct sockaddr_in adrecriv, int sd, struct sockaddr_in adrlect, int erreur, socklen_t adrecriv_len){
    adrecriv_len = sizeof(adrecriv);
    

}

int main() {
    int sd;
    struct sockaddr_in adrlect;
    struct sockaddr_in adrecriv;
    socklen_t adrecriv_len;
    int erreur;
    int nbcar;
    char buff_emission[MAXOCTETS+1];
    char buff_reception[MAXOCTETS+1];
    

    init_server_socket(adrecriv, sd, adrlect, erreur, adrecriv_len);

    sd = socket(AF_INET,SOCK_DGRAM,0);
    CHECK_ERROR(sd, -1, "Erreur socket non cree !!! \n");
    printf("N° de la socket : %d \n", sd);
    
    //preparation de l'adresse de la socket
    adrlect.sin_family = AF_INET;
    adrlect.sin_port = htons(LOCALPORT);
    adrlect.sin_addr.s_addr = inet_addr(LOCALIP);
    
    //Affectation d'une adresse a la socket
    erreur=bind(sd, (const struct sockaddr *) &adrlect, sizeof(adrlect));
    CHECK_ERROR(erreur, -1, "Erreur de bind !!! \n");
    
    while (1){
        printf("Scanning rep\n");

        //reception du message de la part de l'écrivain
        nbcar=recvfrom(sd, buff_reception, MAXOCTETS+1, 0, (struct sockaddr * ) &adrecriv, &adrecriv_len);
        CHECK_ERROR(nbcar, 0, "\nProbleme de reception !!! \n");
        printf("MSG RECU DU CLIENT ADRESSE IP %s > %s\n", inet_ntoa(adrecriv.sin_addr), buff_reception);

        //vérification de la demande de déconnexion
        if (strcmp(buff_emission, "exit") == 0 || strcmp(buff_reception, "exit") == 0){
            CHECK_ERROR(close(sd), -1, "Erreur lors de la fermeture de la socket");
            break;
        }

        //envoie du message à l'écrivain
        printf("SERVEUR> ");
        fgets(buff_emission, MAXOCTETS, stdin);
        buff_emission[strlen(buff_emission)-1] = '\0';
        nbcar = sendto(sd, buff_emission, strlen(buff_emission)+1, 0, (const struct sockaddr *) &adrecriv, sizeof(adrecriv));
        CHECK_ERROR(nbcar, 0, "\nProbleme d'emission !!! \n");

        //vérification de la demande de déconnexion
        if (strcmp(buff_emission, "exit") == 0 || strcmp(buff_reception, "exit") == 0){
            CHECK_ERROR(close(sd), -1, "Erreur lors de la fermeture de la socket");
            break;
        }

    }
    
    exit(EXIT_SUCCESS);
}