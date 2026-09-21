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

    // Temporary raw ADC thresholds until the moisture sensor is calibrated
    // and the user's webpage settings are connected.
    const uint16_t IRRIGATION_DRY_THRESHOLD = 2500;
    const uint16_t IRRIGATION_WET_THRESHOLD = 1800;

    // Irrigation state machine.
    if (Irrigation_State == SolenoidState::OFF) {
        Pump_State = Pump::OFF;
        // The irrigation solenoid should be closed here.
    }
    else if (Irrigation_State == SolenoidState::MANUAL) {
        Pump_State = Pump::ON;
        // The irrigation solenoid should be open here.
    }
    else if (Irrigation_State == SolenoidState::SCHEDULE) {
        // This will become Irrigation_Schedule.isActive() later.
        bool irrigationScheduleActive = false;

        if (irrigationScheduleActive) {
            Pump_State = Pump::ON;
            // The irrigation solenoid should be open here.
        }
        else {
            Pump_State = Pump::OFF;
            // The irrigation solenoid should be closed here.
        }
    }
    else if (Irrigation_State == SolenoidState::AUTO) {
        // This assumes a higher raw reading means drier soil.
        // We will verify that direction during moisture calibration.
        if (Moisture_Raw >= IRRIGATION_DRY_THRESHOLD) {
            Pump_State = Pump::ON;
            // The irrigation solenoid should be open here.
        }
        else if (Moisture_Raw <= IRRIGATION_WET_THRESHOLD) {
            Pump_State = Pump::OFF;
            // The irrigation solenoid should be closed here.
        }
        // Between the thresholds, keep the previous pump state.
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
