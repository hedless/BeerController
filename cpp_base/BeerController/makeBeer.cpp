/*
 * makeBeer.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>
#include "PID.h"
#include "InputFile.h"
#include "Cooler.h"
using namespace std;

int main()
{

	chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch());

	long long int start = ms.count();

	cout << "time: " << start << endl;

	int count = 0;

	string inputPath = "/Users/dhedley/BeerController/sensor_output";
	string inputFileName = "beerlog.txt";
	string outputFileName = "outputBeerLog.dat";

	//date,beer temp, outside temp, set temp, humidity, power state, PINout
	//2015-01-04 20:56:19,67.21,73.40,67,36.00,Off,4748.09

	//Create object of the beer log
	InputFile *beerLog;
	beerLog = new InputFile(inputPath + "/" + inputFileName,
			inputPath + "/" + outputFileName);

	//output number of records found in the log file
	count = beerLog->getCount();

	cout << "Records in output: " << count << endl;

	cout << "MaxTemp: " << beerLog->getMaxBeerTemp() << endl;
	cout << "MinTemp: " << beerLog->getMinBeerTemp() << endl;
	cout << "Max Outside Temp: " << beerLog->getMaxOutsideTemp() << endl;
	cout << "Min Outside Temp: " << beerLog->getMinOutsideTemp() << endl;
	cout << "Max Humidity: %" << beerLog->getMaxHumidity() << endl;
	cout << "Min Humidity: %" << beerLog->getMinHumidity() << endl;
	cout << "Max temp difference between outside and beer: "
			<< beerLog->getMaxTempDiff() << endl;
	cout << "Max Set temperature was: " << beerLog->getMaxSetTemp() << endl;
	cout << "Min Set temperature was: " << beerLog->getMinSetTemp() << endl;
	cout << "Maximum the temp was off from set point: "
			<< beerLog->getMaxTempOff() << endl;
	cout << "Average difference from set point: "
			<< beerLog->getAverateTempOff() << endl;









	double Setpoint, Input, Output;

	Setpoint = 65;

	PID myPID(&Input, &Output, &Setpoint, 1, 40, 1, DIRECT);

	chrono::milliseconds msafter = chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch());

	long windowStartTime = msafter.count();
	//tell the PID to range between 0 and the full window size
	int WindowSize = 5000;
	myPID.SetOutputLimits(0, WindowSize);

	//turn the PID on
	myPID.SetMode(AUTOMATIC);

	//starting temp point
	double temperature = 70;
	double fridgeUpSpeed = 0.1;
	double fridgeDownSpeed = 0.2;
	double fridgeMaxTempVariance = 50;

	//set up fridge to run with
	Cooler fridge(temperature, fridgeUpSpeed, fridgeDownSpeed,
			fridgeMaxTempVariance);
	int sampleTime = 100; //sample time in milliseconds

	myPID.SetSampleTime(sampleTime);

	int minDuration = 10; //insure that the fridge doesn't get turned on and off to frequently

	while (true)
	{
		temperature = fridge.getTemperature();
		Input = temperature;
		myPID.Compute();

		std::this_thread::sleep_for(std::chrono::milliseconds(sampleTime));

		cout << "fridge temp: " << temperature << " Output is: " << Output
				<< " Fridge is on?: " << fridge.getPowerStatus() << endl;

		chrono::milliseconds msecs =
				chrono::duration_cast<chrono::milliseconds>(
						chrono::system_clock::now().time_since_epoch());

		long millis = msecs.count();

		if (millis - windowStartTime > WindowSize)
		{ //time to shift the Relay Window
			windowStartTime += WindowSize;
		}

		if (!(minDuration > 0))
		{
			if (Output < millis - windowStartTime)
			{
				fridge.turnPowerOff();
			}
			else
			{
				fridge.turnPowerOn();
			}
			minDuration = 10; //set back to minDuration
		}
		else
		{
			minDuration--;
		}

	}

}

