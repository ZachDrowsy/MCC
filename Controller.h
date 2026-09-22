#pragma once

#include "States.h"
#include "Auto_Control.h"
#include "Schedule.h"
#include "Sensor.h"

class Controller{
private: 
  bool Should_Run = false;
  bool manual = false; 
  Schedule schedule;
  AutoControl automatic;
/* Declared in States.h
SolenoidState Irrigation_State = SolenoidState::OFF;
SolenoidState Mist_State = SolenoidState::OFF;
LightState Light_State = LightState::OFF;
DeviceState Fan_State = DeviceState::OFF;
DeviceState Heater_State = DeviceState::OFF;
*/

public:
// use 
  Controller();
  //Controller(const AutoControl& A, const Schedule& S, const Should_Run& = false, const manual& = false); 
// For Light
  Controller(const Schedule& S,bool SH, bool m); 
// For Device state (Heater and fan)
  Controller(const AutoControl& A, bool SH, bool m);
//For Pump
  Controller(const bool SH);

void update

};
