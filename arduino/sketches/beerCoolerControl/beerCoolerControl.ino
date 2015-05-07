#include <PID_v1.h>
#include <OneWire.h> 
#include "DHT.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

 int epromAddr = 0;

 int RelayPin=13;
//#define RelayPin 10


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int  buttonPin1 = 8;
const int  buttonPin2 = 9;
int temperatureInitialSetting = 70;
int button1State=0;
int button2State=0;
int lastButton1State=0;
int lastButton2State=0;
int timeCountMill=0;
int timeToCheck=2000; //set number of loops before measureing temp


int swapDisplay=0;
int DS18S20_Pin = 7; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2


#define DHTPIN 6 // set up humidity sensor on pin 4
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,1,40,1, DIRECT);

int WindowSize = 5000;
//int minWindow = 2500;


unsigned long windowStartTime;

void setup(void) {
  Serial.begin(9600);
  lcd.begin(16, 2);
  //lcd.print("hello, world!");
  dht.begin();
  pinMode(buttonPin1,INPUT);
  pinMode(buttonPin2,INPUT);
  
  pinMode(RelayPin, OUTPUT);


  windowStartTime = millis();


  //initialize the variables we're linked to
  int eepromVal = EEPROM.read(epromAddr);
  if ( eepromVal != 255) {
    temperatureInitialSetting = eepromVal;
  }
  Setpoint = temperatureInitialSetting;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop(void) {
  //read the pushbutton input pins:
  button1State = digitalRead(buttonPin1);
  button2State = digitalRead(buttonPin2);

  // compare the buttonState to its previous state
  if (button1State != lastButton1State) {
    // if the state has changed, increment the counter
    if (button1State == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      temperatureInitialSetting++;
      //Serial.println("up");
      //Serial.print("new temp setting:  ");
      //Serial.println(temperatureInitialSetting);
    } 
    
  }
  lastButton1State = button1State;

  if (button2State != lastButton2State) {
    // if the state has changed, increment the counter
    if (button2State == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      temperatureInitialSetting--;
      //Serial.println("down");
      //Serial.print("new temp setting:  ");
      //Serial.println(temperatureInitialSetting);
    } 
   
  }
  lastButton2State = button2State;
  delay(2);
  timeCountMill++;
  
  
  if (timeCountMill>=timeToCheck){




    lcd.setCursor(0, 0);
    float temperature = getTemp();
    temperature = (temperature * 1.8) + 32;
    //Serial.println("onewire temp");
    //Serial.println(temperature);
    lcd.print("Cooler:");
    lcd.print(temperature);
    lcd.print("F     ");

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    //convert to F
    t = (t * 1.8) +32;

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
    } 
    else {
      //Serial.print("Humidity: "); 
      //Serial.print(h);
      //Serial.print(" %\t");
      //Serial.print("Temperature: "); 
      Serial.print(temperature);
      Serial.print(",");
      Serial.print(t);
      Serial.print(",");
      Serial.print(temperatureInitialSetting);
      Serial.print(",");
      Serial.print(h);

      //Serial.println(" *F");

      if (swapDisplay==0){
        lcd.setCursor(0, 1);
        //lcd.print("O:");
        lcd.print(t);
        lcd.print("F");
        lcd.print(" H:");
        lcd.print(h);
        lcd.print("%");
        swapDisplay=1;
      } 
      else {
        lcd.setCursor(0, 1);
        lcd.print("Temp Set:");
        lcd.print(temperatureInitialSetting); 
        lcd.print("     ");
        swapDisplay=0;
      }
    }


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
      digitalWrite(RelayPin,LOW);
      Serial.print(",On,");
      Serial.print(Output);


    }
    else {
      digitalWrite(RelayPin,HIGH);
      Serial.print(",Off,");
      Serial.print(Output);
    }

    if (Setpoint != temperatureInitialSetting) {
      Setpoint = temperatureInitialSetting; 
      EEPROM.write(epromAddr, Setpoint);
      
    }

    Serial.print("\n");  //finish log line

    timeCountMill=0;
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);

  // lcd.print("hello, world!");

  //delay(2000); //just here to slow down the output so it is easier to read

}


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}



