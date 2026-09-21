#include "connection.h"
#include "States.h"
#include "Schedule.h"
#include "Sensors.h"

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

    // Irrigation state machine.
    if (Irrigation_State == SolenoidState::OFF) {
        // The irrigation output should stay off.
        // We still need an irrigation relay pin before we can write hardware here.
    }
    else if (Irrigation_State == SolenoidState::MANUAL) {
        // Manual mode will open irrigation when the user asks for it.
        // We still need the relay pin and a manual on/off command.
    }
    else if (Irrigation_State == SolenoidState::SCHEDULE) {
        // Schedule mode will open irrigation while the schedule is active.
        // We have the Schedule class, but we still need an irrigation
        // Schedule object and settings for its start time, duration, and days.
    }
    else if (Irrigation_State == SolenoidState::AUTO) {
        // Automatic mode will use the soil moisture reading.
        // We have the AutoControl class, but we still need to choose the
        // moisture thresholds and connect its result to the irrigation relay.
    }

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
