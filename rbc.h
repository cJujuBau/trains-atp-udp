#include <stdio.h>
#include <string.h>


#define TRACKLENGTH 10000

typedef struct{
    char no_train[20];
    int loc;
    int eoa;
} trainData;

trainData empty_train;

void add_train(trainData trainRegister[], char * name){
    if ((trainRegister[0].loc == 0) && (trainRegister[0].no_train[0] != 0)) {
        printf("Cannot add a train : there is already %s at loc 0 !\n", trainRegister[0].no_train);
        return;
    }
    for(int i = 1;i<TRACKLENGTH;i++){
        trainRegister[TRACKLENGTH-i] = trainRegister[TRACKLENGTH-(i+1)];
    }
    trainData new_train;
    strcpy(new_train.no_train, name);
    new_train.loc = 0;
    new_train.eoa = 0;
    printf("in check train %s\n", new_train.no_train);
    trainRegister[0] = new_train;
    if (trainRegister[1].loc != -1){
        trainRegister[0].eoa = trainRegister[1].loc - 1;
    }
    else{
        trainRegister[0].eoa = TRACKLENGTH;
    }
}

void remove_train(trainData trainRegister[]){
    for(int i = 0;i<TRACKLENGTH;i++){
        if (trainRegister[i].loc == TRACKLENGTH){
            trainRegister[i] = empty_train;
        }
    }
}

void display_trains(trainData trainRegister[]){
    for (int i = 0;i<TRACKLENGTH;i++){
        if (trainRegister[i].no_train[0] != 0){
            printf("Train no %s at loc %d and eoa %d\n", trainRegister[i].no_train, trainRegister[i].loc, trainRegister[i].eoa);
        }
    }
}

void init_register(trainData trainRegister[]){
    strcpy(empty_train.no_train, "");
    empty_train.loc = -1;
    empty_train.eoa = -1;
    for (int i = 0;i<TRACKLENGTH;i++){
        trainRegister[i] = empty_train;
    }
}

int ask_permission(trainData trainRegister[], int old_location, int new_location){
    int i = 0;
    while ((trainRegister[i].loc != old_location) && (i < TRACKLENGTH)){
        i++;
    }
    if (i == TRACKLENGTH){
        printf("Found no train at %d\n", old_location);
        return 0;
    }
    else{
        if (new_location > trainRegister[i].eoa){
            return 0;
        }
        return 1;
    }
}

void move_train(trainData trainRegister[], int old_location, int new_location){
    int i = 0;
    while (trainRegister[i].loc != old_location && i < TRACKLENGTH){
        i++;
    }

    if (i == TRACKLENGTH){
        printf("Found no train on loc %d\n", old_location);
    }
    else{
        if (!ask_permission(trainRegister, old_location, new_location)){
            printf("Did not get authorization to move train %s\n", trainRegister[i].no_train);
            return;
        }
        trainRegister[i].loc = new_location;
        printf("Successfully moved train %s from %d to %d\n", trainRegister[i].no_train, old_location, trainRegister[i].loc);
        if (i!=0){
            trainRegister[i-1].eoa = new_location - 1;
        }
    }
}

void parse_msg(char * msg){
    
}