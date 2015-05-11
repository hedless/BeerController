/*
 * runpid.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */


#include "PID.h"

int millis() {return 0;};
int delay(int){return 0;};

int timeCountMill=0;
int timeToCheck=2000; //set number of loops before measureing temp


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,1,40,1, DIRECT);

int WindowSize = 5000;
//int minWindow = 2500;


unsigned long windowStartTime;

void setup(void) {

  windowStartTime = millis();


  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop(void) {


  delay(2);
  timeCountMill++;


  if (timeCountMill>=timeToCheck){

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
   // float h = dht.readHumidity();
    //float t = dht.readTemperature();
    //convert to F
    //t = (t * 1.8) +32;
	  int temperature = 0; //need to have this change
    //set PID controller
    Input = temperature;
    myPID.Compute();

    /************************************************
     * turn the output pin on/off based on pid output
     ************************************************/
    if(millis() - windowStartTime>WindowSize)
    { //time to shift the Relay Window
      windowStartTime += WindowSize;
    }
    if(Output < millis() - windowStartTime)
    {
//      digitalWrite(RelayPin,LOW);
//      Serial.print(",On,");
//      Serial.print(Output);


    }
    else {
//      digitalWrite(RelayPin,HIGH);
//      Serial.print(",Off,");
//      Serial.print(Output);
    }

//    if (Setpoint != temperatureInitialSetting) {
//      Setpoint = temperatureInitialSetting;
//      EEPROM.write(epromAddr, Setpoint);

//    }

  //  Serial.print("\n");  //finish log line

    timeCountMill=0;
  }

}




