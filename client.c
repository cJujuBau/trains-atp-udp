#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define CHECK_ERROR(val1, val2, msg)	if (val1==val2) { printf("%s \n", msg) ; exit(EXIT_FAILURE); }
#define MAXCARS	80

#define LOCALIP     "172.20.10.3"

//train:command:option
//ter500:reg/for/unr:./52/.

// ./client ipserv portserv

int main (int argc, char ** argv)
{
    int sd; //socket de dialogue
    struct sockaddr_in addr_evc, addr_rbc;
    int longaddr;
    int nbcar; 

    int code_erreur;

    char recept[MAXCARS+1]; //buffer de reception
    char emiss[MAXCARS+1]; //buffer d'emission

    //Etape 1 : creation de la socket
    sd=socket(AF_INET, SOCK_DGRAM, 0); //creation d'un descripteur de socket pour communiquer avec des processus localises sur la même machine.
    CHECK_ERROR(sd,-1, "la creation de la socket");

    //Etape 2 : Je definis l'adresse de la socket

    addr_evc.sin_family=AF_INET;
    //addr_evc.sin_port = 2000; 
    addr_rbc.sin_family=AF_INET;

    addr_evc.sin_addr.s_addr = inet_addr(LOCALIP);

    if (argc < 3){
        printf("usage %s <adresse ip rbc> <port rbc>", argv[0]); 
        return EXIT_FAILURE; 
    }
    else{
        addr_rbc.sin_addr.s_addr = inet_addr (argv[1]);
        
        u_int16_t port  = htons(atoi(argv[2])); 
        addr_rbc.sin_port = port; 
        printf("adresse ip : %s\n", inet_ntoa(addr_rbc.sin_addr)); 
    }

    //Etape 3 : affectation de l'adresse à la socket

    code_erreur = bind(sd, (struct sockaddr *)&addr_evc, sizeof(addr_evc));
    CHECK_ERROR(code_erreur,-1,"bind");


    
    do{    
        //Etape 4 : emission d'une reponse
        printf("evc> ");

        longaddr=sizeof(struct sockaddr_in);

        fgets(emiss,MAXCARS,stdin);
        emiss[strlen(emiss)-1]='\0'; //suppression du retour chariot qui est insere dans le buffer par fgets

        nbcar=sendto(sd,emiss, strlen(emiss), 0,(struct sockaddr *) &addr_rbc, longaddr);

        if (nbcar < 0) {
            perror("Erreur lors de l'envoi redacteur");
            continue; // ou exit(EXIT_FAILURE);
        }

        //Etape 5 : reception de data
        nbcar=recvfrom(sd,recept,MAXCARS+1, 0, (struct sockaddr *) &addr_rbc, &longaddr);

        if (nbcar > 0) {
                recept[nbcar] = '\0'; // Terminer la chaîne reçue
                printf("RBC IP:%s> '%s'\n", inet_ntoa(addr_rbc.sin_addr),recept);          
            }

    } 
    while (strcmp(recept, "end") && strcmp(emiss,"end"));
    //while (strcmp(emiss,"end"));

    getchar(); 

    CHECK_ERROR(close(sd), -1, "Erreur lors de la fermeture de la socket");
    exit(EXIT_SUCCESS);
}
