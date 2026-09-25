#include "include/connection.h"
#include "include/States.h"
#include "include/Schedule.h"
#include "include/Sensors.h"
#include "include/Controller.h"
#include "include/Auto_Control.h"


const uint8_t PUMP_PIN = 32;

AutoControl Auto;
Schedule schedule;

Controller Irrigation(Auto, schedule);
Controller Mist(Auto, schedule);
Controller Light(schedule);
Controller Heater(Auto);
Controller Fan(Auto);

void setup(){
    // Keep the pump and valves off before sensors and Wi-Fi start.
    digitalWrite(PUMP_PIN, RELAY_OFF);
    pinMode(PUMP_PIN, OUTPUT);
    Setup_Relays();

    Serial.begin(115200);
    
    // sets up declarations for all THREE sensors 
    Setup_Sensors(); // moisture pin = MOISTURE_PIN
    Register_State_Controls(server);
    startMCCNetwork();
}

void loop(){
    handleMCCNetwork();
    
    //This will read and display sensor data in the Serial Monitor and hopefully the web_ui
    Read_Sensors();
    Irrigation.update(Irrigation_Activation, Irrigation_State, Moisture_Raw);
    Mist.update(Mist_Activation, Mist_State, humidity);
    Light.update(Light_Activation, Light_State);
    Heater.update(Heater_Activation, Heater_State, temp);
    Fan.update(Fan_Activation, Fan_State, temp);

    // Temporary raw ADC thresholds until the moisture sensor is calibrated
    // and the user's webpage settings are connected.
    const uint16_t IRRIGATION_DRY_THRESHOLD = 2500;
    const uint16_t IRRIGATION_WET_THRESHOLD = 1800;

    // Irrigation and mist share one pump.
    if (Irrigation.getShouldRun() == true || Mist.getShouldRun() == true) {
        Pump_State = Pump::ON;

        if (Irrigation.getShouldRun() == true && Mist.getShouldRun() == true) {
            Irrigation.Run_Irrigation(Pump::ON);
            Mist.Run_Mist(Pump::ON);
        }
        else if (Irrigation.getShouldRun() == true) {
            Irrigation.Run_Irrigation(Pump::ON);
            Mist.Run_Mist(Pump::OFF);
        }
        else if (Mist.getShouldRun() == true) {
            Irrigation.Run_Irrigation(Pump::OFF);
            Mist.Run_Mist(Pump::ON);
        }
    }
    else {
        Pump_State = Pump::OFF;

        Irrigation.Run_Irrigation(Pump::OFF);
        Mist.Run_Mist(Pump::OFF);
    }

    // Control the shared pump once.
    if (Pump_State == Pump::ON) {
        digitalWrite(PUMP_PIN, RELAY_ON);
    }
    else {
        digitalWrite(PUMP_PIN, RELAY_OFF);
    }

// Run light state decision
Light.Run_Light();
// Heater and Fan state decision
Heater.Run_Heater();
Fan.Run_Fan();


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
