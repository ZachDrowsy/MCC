#include "connection.h"
#include "States.h"
#include "Schedule.h"

#include <Wire.h> // Needed for handling the intricacies of handling I2C specifically for our AH30 sensor
#include <SparkFun_Qwiic_Humidity_AHT20.h> // #1 Library for the Humidity/Temperature sensor used

const uint8_t MOISTURE_PIN = 3;

// for reading moisture level
uint16_t Moisture_Raw = 0;
   
// Sensor object and declaration pertaining to it
AHT20 HTsensor;
float temp = 0.0;
float humidity = 0.0;
bool Sensor_Connected = false;

void setup(){
    
    Serial.begin(115200);
    //I2C for AHT30
    Wire.begin();

    // Analog input for soil moisture sensor
    pinMode(MOISTURE_PIN, INPUT);

    startMCCNetwork();
    if(!HTsensor.begin()){
        Serial.print("Sensor Not Found!");
        while(1);
    }else{
        Serial.println("Sensor, AH30 is Connected");
        Sensor_Connected = true;
    }

}

void loop(){
    handleMCCNetwork();
    
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

    // Get moisture level data;
    Moisture_Raw = analogRead(MOISTURE_PIN);

    // Get AHT30 data
    if(HTsensor.available()){
        temp = HTsensor.getTemperature();
        humidity = HTsensor.getHumidity();

        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print(" | ");
        Serial.print("Humidity: ");
        Serial.println(humidity);



    }
    //
}
