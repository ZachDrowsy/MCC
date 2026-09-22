#pragma once

#include <Arduino.h>
#include "connection.h" // Uses the same web server as the main website.

#include <cmath>
#include <Wire.h> // Needed for handling the intricacies of handling I2C specifically for our AH30 sensor
#include <SparkFun_Qwiic_Humidity_AHT20.h> // #1 Library for the Humidity/Temperature sensor used

const uint8_t MOISTURE_PIN = 34; // ADC1 input works while Wi-Fi is running

// for reading moisture level
uint16_t Moisture_Raw = 0;
   
// Sensor object and declaration pertaining to it
AHT20 HTsensor;
float temp = 0.0;
float humidity = 0.0;
bool Sensor_Connected = false;

// Do not send startup values or old AHT30 readings as live data.
bool Sensor_Ready = false;
unsigned long Last_Sensor_Read = 0;

void Update_Sensors(){
    server.on("/api/sensors", HTTP_GET, [](){
        bool reading_Ready = Sensor_Ready && millis() - Last_Sensor_Read < 5000;

        String data = "{\"temperatureF\":";
        if(reading_Ready){
            data += String(temp * 9.0 / 5.0 + 32.0, 1);
        }else{
            data += "null";
        }

        data += ",\"humidity\":";
        if(reading_Ready){
            data += String(humidity, 1);
        }else{
            data += "null";
        }

        // Raw ADC value until dry and wet soil readings are calibrated.
        data += ",\"moistureRaw\":";
        data += String(Moisture_Raw);
        data += "}";
        server.sendHeader("Cache-Control", "no-store");
        server.send(200, "application/json", data);
    });
}

// Call once from setup to prepare the sensors and their web endpoint.
void Setup_Sensors(){
    //I2C for AHT30
    Wire.begin();

    // Analog input for soil moisture sensor
    pinMode(MOISTURE_PIN, INPUT);

    Moisture_Raw = analogRead(MOISTURE_PIN);
    Update_Sensors();
    if(!HTsensor.begin()){
        Serial.print("Sensor Not Found!");
        // Keep the website and soil moisture reading working without the AHT30.
    }else{
        Serial.println("Sensor, AH30 is Connected");
        Sensor_Connected = true;
    }
}

// Call from loop to keep the stored readings current.
void Read_Sensors(){
    // Get moisture level data;
    Moisture_Raw = analogRead(MOISTURE_PIN);

    // Get AHT30 data
    if(Sensor_Connected && HTsensor.available()){
        if(!HTsensor.isConnected()){
            Sensor_Ready = false;
            return;
        }

        temp = HTsensor.getTemperature();
        humidity = HTsensor.getHumidity();
        Sensor_Ready = std::isfinite(temp) && std::isfinite(humidity);
        Last_Sensor_Read = millis();

        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print(" | ");
        Serial.print("Humidity: ");
        Serial.println(humidity);



    }
}
