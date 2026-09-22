#include "include/connection.h"
#include "include/States.h"
#include "include/Schedule.h"
#include "include/Sensors.h"
#include "include/Controller.h"
#include "include/Auto_Control.h"


AutoControl Auto;
Schedule schedule;

Controller Irrigation(Auto, schedule);
Controller Mist(Auto, schedule);
Controller Light(schedule);
Controller Heater(Auto);
Controller Fan(Auto);

void setup(){
    Serial.begin(115200);
    
    // sets up declarations for all THREE sensors 
    Setup_Sensors(); // moisture pin = MOISTURE_PIN
    Register_State_Controls();
    startMCCNetwork();
}

void loop(){
    handleMCCNetwork();
    
    //This will read and display sensor data in the Serial Monitor and hopefully the web_ui
    Read_Sensors();
    Irrigation.update(Irrigation_State, Moisture_Raw);
    Mist.update(Mist_State, humidity);
    Light.update(Light_State);
    Heater.update(Heater_State, temp);
    Fan.update(Fan_State, tempe);

    // Temporary raw ADC thresholds until the moisture sensor is calibrated
    // and the user's webpage settings are connected.
    const uint16_t IRRIGATION_DRY_THRESHOLD = 2500;
    const uint16_t IRRIGATION_WET_THRESHOLD = 1800;

    // Irrigation state machine.
Irrigation.update(Irrigation_State, Moisture_Raw);

if (Irrigation.getShouldRun() == true) {
    // Open irrigation valve.
} else {
    // Close irrigation valve.
}
        // This assumes a higher raw reading means drier soil.
        // We will verify that direction during moisture calibration.
   

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
