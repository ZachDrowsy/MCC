#include "connection.h"
#include "States.h"
#include "Schedule.h"
#include "Sensors.h"

void setup(){
    Serial.begin(115200);
    
    // sets up declarations for all THREE sensors 
    Setup_Sensors(); // moisture pin = MOISTURE_PIN
    startMCCNetwork();
}

void loop(){
    handleMCCNetwork();
    
    //This will read and display sensor data in the Serial Monitor and hopefully the web_ui
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
