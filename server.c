#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rbc.h"

#define CHECK_ERROR(val1,val2,msg)   if (val1==val2) \
                                    { perror(msg); \
                                        exit(EXIT_FAILURE); }

// To use : ./server <ip> <port>

#define MAXOCTETS   150

void init_server_socket(struct sockaddr_in * adrecriv, int * sd, struct sockaddr_in * adrlect, int * erreur, char * local_ip, int local_port){
    // init_server_socket(adrecriv, sd, adrlect, erreur);
    *sd = 1;
    printf("Test\n");
    *sd = socket(AF_INET,SOCK_DGRAM,0);
    CHECK_ERROR(*sd, -1, "Erreur socket non cree !!! \n");
    printf("N° de la socket : %d \n", *sd);
    
    printf("Adrlect %d port %d\n", inet_addr(local_ip), htons(local_port));
    
    // preparation de l'adresse de la socket
    adrlect->sin_family = AF_INET;
    adrlect->sin_port = htons(local_port);
    adrlect->sin_addr.s_addr = inet_addr(local_ip);
    
    // Affectation d'une adresse a la socket
    *erreur=bind(*sd, (const struct sockaddr *) adrlect, sizeof(*adrlect));
    CHECK_ERROR(*erreur, -1, "Erreur de bind !!! \n");
}

void scan_msg(char buffer[MAXOCTETS+1], struct sockaddr_in * adrecriv, int * sd){
    char buff_emission[MAXOCTETS+1];
    int nbcar;
    socklen_t adrecriv_len;
    adrecriv_len = sizeof(adrecriv);
    //reception du message de la part de l'écrivain
    nbcar=recvfrom(*sd, buffer, MAXOCTETS+1, 0, (struct sockaddr * ) adrecriv, &adrecriv_len);
    CHECK_ERROR(nbcar, 0, "\nProbleme de reception !!! \n");
    printf("MSG RECU DU CLIENT ADRESSE IP %s > %s\n", inet_ntoa(adrecriv->sin_addr), buffer);

    //vérification de la demande de déconnexion
    if (strcmp(buff_emission, "exit") == 0 || strcmp(buffer, "exit") == 0){
        CHECK_ERROR(close(*sd), -1, "Erreur lors de la fermeture de la socket");
        return;
    }
}

int main(int argc, char * argv[]) {
    if (argc < 3){
        printf("Missing arguments. Use ./server <ip> <port>\n");
        exit(0);
    }
    char * local_ip = argv[1];
    int local_port = atoi(argv[2]);
    printf("%s, %d\n", local_ip, local_port);
    int sd;
    int erreur;
    struct sockaddr_in adrlect;
    struct sockaddr_in adrecriv;

    init_server_socket(&adrecriv, &sd, &adrlect, &erreur, local_ip, local_port);

    printf("Adrlect %d port %d\n", adrlect.sin_addr.s_addr, adrlect.sin_port);
    
    
    trainData trainRegister[TRACKLENGTH];
    
    init_register(trainRegister);

    add_train(trainRegister, "TGV0");

    display_trains(trainRegister);

    move_train(trainRegister, 0,10);

    add_train(trainRegister, "TGV1");

    move_train(trainRegister, 10, 50);

    move_train(trainRegister, 0, 30);

    add_train(trainRegister, "TER0");

    move_train(trainRegister, 0, 100);

    move_train(trainRegister, 400, 700);
    
    parse_msg(trainRegister, "TGV0:4:50:150:");

    fprintf(stderr, "test");
    
    while (1) {
        char buffer[MAXOCTETS + 1] = {""};
        scan_msg(buffer, &adrecriv, &sd); // Réception du message

        if (strcmp(buffer, "exit") == 0) {
            printf("Déconnexion demandée par le client.\n");
            break;
        }

        // Préparer une réponse et l'envoyer
        char response[MAXOCTETS + 1];
        snprintf(response, sizeof(response), "Message reçu : %s", buffer);

        answer(response, &adrecriv, &sd); // Envoi de la réponse au client
    }

    exit(EXIT_SUCCESS);
}

void answer(const char *message, struct sockaddr_in *adrecriv, int *sd) {
    socklen_t adrecriv_len = sizeof(*adrecriv); // Taille de la structure sockaddr_in
    int nbcar;

    // Envoi du message au client
    nbcar = sendto(*sd, message, strlen(message), 0, (struct sockaddr *)adrecriv, adrecriv_len);
    CHECK_ERROR(nbcar, -1, "Erreur lors de l'envoi du message");

    printf("Réponse envoyée au client [%s:%d] : %s\n",
           inet_ntoa(adrecriv->sin_addr), ntohs(adrecriv->sin_port), message);
}