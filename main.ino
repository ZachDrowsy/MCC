#include "connection.h"
#include "States.h"
#include "Schedule.h"
#include "Sensors.h"

void setup(){
    Serial.begin(115200);
    Setup_Sensors();
    startMCCNetwork();
}

void loop(){
    handleMCCNetwork();
    Read_Sensors();

    //Display states to error check 
    /*Serial.print("Pump State: ");
    Serial.print(Pump_State);
    Serial.print(" |              ");
    Serial.print()
    .....\/
    .....\/
    Serial.println(last state you want displayed)
    */ 
    // START WITH OFF CONDITIONS
}
