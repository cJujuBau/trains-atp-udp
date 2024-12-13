#include "rbc.h"

int main(){
    strcpy(empty_train.no_train, "");
    empty_train.loc = -1;
    empty_train.eoa = -1;

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

    display_trains(trainRegister);
}