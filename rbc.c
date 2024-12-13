#include "rbc.h"

int main(){
    trainData trainRegister[TRACKLENGTH];
    init_register(trainRegister);

    parse_msg(trainRegister, "TGV0:1:"); // Add train

    display_trains(trainRegister);

    parse_msg(trainRegister, "TGV0:4:10:"); // Move train

    parse_msg(trainRegister, "TGV1:1:"); // Add train

    parse_msg(trainRegister, "TGV0:4:50:"); // Move train

    parse_msg(trainRegister, "TGV1:4:30:"); // Move train

    parse_msg(trainRegister, "TER0:1:"); // Add train

    parse_msg(trainRegister, "TER0:4:100:"); // Move train, illegal should be rejected

    parse_msg(trainRegister, "TER0:3:100:"); // Ask permission to move train, illegal should be rejected

    parse_msg(trainRegister, "TGV0:4:150:"); // Move train

    parse_msg(trainRegister, "TGV0:4:1000:"); // Move train

    parse_msg(trainRegister, "TGV0:2:"); // Move train

    display_trains(trainRegister);

    /*
    Final state should be :
    Train no TER0 at loc 0 and eoa 29
    Train no TGV1 at loc 30 and eoa 999
    */
}