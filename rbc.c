#include "rbc.h"

int main(){
    trainData trainRegister[TRACKLENGTH];
    init_register(trainRegister);

    add_train(trainRegister, "TGV0");

    display_trains(trainRegister);

    move_train(trainRegister, "TGV0",10);

    add_train(trainRegister, "TGV1");

    move_train(trainRegister, "TGV0", 50);

    move_train(trainRegister, "TGV1", 30);

    add_train(trainRegister, "TER0");

    move_train(trainRegister, "TER0", 100);

    parse_msg(trainRegister, "TGV0:4:150:");

    display_trains(trainRegister);
}