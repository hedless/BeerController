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
#include <iomanip>
#include <windows.h>
#include "PID.h"
#include "InputFile.h"
#include "Cooler.h"
using namespace std;

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

void runController(Cooler &unit); //to run the controller
long getMillisec(); //get current time in milliseconds
void displayStats(InputFile *);  //display stats for an input file

string inputPath = "C:\\work\\CodeBlocks\\BeerController\\BeerController\\BeerController\\sensor_output"; //location for the sensor output info
string inputFileName = "beerlog.txt"; //text log file to crunch through for stats
string outputFileName = "outputBeerLog.dat"; //binary version of the log file created by InputFile class
string newBeerLog = "newBeer.log"; //newBeerlog created by the controller program
int dTime = 1; //Set for number of loops before it switches power again to keep from turning the cooler on and off too fast
int desiredBeerTemp = 65; //default temperature you want your beer set
int sampleTime = 1000; //sample time in milliseconds, how often it reads temp

int main()
{

	cout << "Welcome to the Beer Controller." << endl;

	bool quit = false; //used for loop

    //Allow user to choose to get stats from a log file or run the controller
	while (!quit)
	{

		bool moveOn = false;
		char input;

        //get user input, check for proper input
		do
		{
			cout
					<< "Please select an option. For Stats from a log file Press[S], to control a cooling unit Press[C]: ";
			cin >> input;
			cin.ignore();
			input = tolower(input);
			if (input == 'c' || input == 's')
				moveOn = true;
			else
				cout << "Sorry, Invalid input please try again..." << endl;

		} while (!moveOn);

		if (input == 's')
		{
			string userFileName;
			cout << "What file do you want to load(must be located in this dir: " << inputPath << ")?: ";
			getline(cin,userFileName);

			//Create object of the beer log
			InputFile *beerLog;

			beerLog = new InputFile(inputPath + kPathSeparator + userFileName,
					inputPath + kPathSeparator + userFileName + ".dat");

			//output number of records found in the log file
			displayStats(beerLog);
		}
		else if (input == 'c')
		{

            cout << "What temp do you want the beer at?: ";
            cin >> desiredBeerTemp;
            cin.ignore();

            int type = 0;
            while(!(type == 1 || type == 2))
            {
                cout << "Which cooler did you want to use? [1] for Fridge, [2] for Custom Cooler: ";
                cin >> type;
                cin.ignore();
            }

            double temperature, fridgeUpSpeed, fridgeDownSpeed, fridgeMaxTempVariance;

            if(type == 1){
                //set for a fridge that can cool quickly
                temperature = 70; //starting temp point
                fridgeUpSpeed = 0.1;
                fridgeDownSpeed = 0.2;
                fridgeMaxTempVariance = 50;
            }
            else
            {
                //set for a custom cooler that cools slowly
                temperature = 70; //starting temp point
                fridgeUpSpeed = 0.1;
                fridgeDownSpeed = 0.1;
                fridgeMaxTempVariance = 20;  //not used yet but can affect cooler
            //TODO: update here for future improvement on temp controlling

            }

			//set up fridge to run with
			Cooler fridge(temperature, fridgeUpSpeed, fridgeDownSpeed,
					fridgeMaxTempVariance);

			runController(fridge);
		}

	}
}

void runController(Cooler &unit)
{
	double Setpoint, Input, Output;
	Setpoint = desiredBeerTemp;
	double humidity = 43.00; //set for now until sensor is put in place
	double outsidTemp = 74.00; //set as 74 by air conditioner
	int minDuration = dTime; //insure that the fridge doesn't get turned on and off to frequently
	int WindowSize = 5000;
	long windowStartTime = getMillisec();

	//logfile for storing new run
	fstream outLogFile;
	string newLog = inputPath + kPathSeparator + newBeerLog;
    outLogFile.open(newLog.c_str(), ios::out | ios::app);

	//setup PID with specs
	PID myPID(&Input, &Output, &Setpoint, 1, 40, 1, DIRECT);

	//tell the PID to range between 0 and the full window size
	myPID.SetOutputLimits(0, WindowSize);

	//turn the PID on
	myPID.SetMode(AUTOMATIC);

	myPID.SetSampleTime(sampleTime);

	while (true)
	{
		double temperature = unit.getTemperature(); //get current cooler inside temp
		Input = temperature;  //tells PID what the temp is
		myPID.Compute(); //computes the error of it being off and will update output on what to do

		//to pause the time interval for when a temperature check needs to take place
		Sleep(sampleTime);
		//this_thread::sleep_for(std::chrono::milliseconds(sampleTime));

		//Get the current time for logging info
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);

		//set up date field for posting ot log
		//std::string date = to_string(now->tm_year + 1900) + "-"
		//		+ to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday)
		//		+ " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min)
		//		+ ":" + to_string(now->tm_sec);

		//Set string representation of power being on or off
		string powerState;
		if (unit.getPowerStatus())
			powerState = "On";
		else
			powerState = "Off";

		//set precision to show 2 decimal places for general output
		cout << setprecision(2) << fixed;
		cout << (now->tm_year + 1900) << "-"
				<< (now->tm_mon + 1) << "-" << now->tm_mday
				<< " " << now->tm_hour << ":" << now->tm_min
				<< ":" << now->tm_sec << "," << temperature << "," << outsidTemp << ","
				<< Setpoint << "," << humidity << "," << powerState << ","
				<< Output << endl;

		//log the current output
		outLogFile << setprecision(2) << fixed;
		outLogFile << (now->tm_year + 1900) << "-"
				<< (now->tm_mon + 1) << "-" << now->tm_mday
				<< " " << now->tm_hour << ":" << now->tm_min
				<< ":" << now->tm_sec << "," << temperature << "," << outsidTemp << ","
				<< Setpoint << "," << humidity << "," << powerState << ","
				<< Output << endl;

		//date,beer temp, outside temp, set temp, humidity, power state, PINout
		//2015-01-04 20:56:19,67.21,73.40,67,36.00,Off,4748.09

		long millis = getMillisec();

		if (millis - windowStartTime > WindowSize)
		{ //time to shift the Relay Window
			windowStartTime += WindowSize;
		}

		if (!(minDuration > 0)) //to keep it from turning the cooler on and off to fast
		{
			if (Output < millis - windowStartTime)
			{
				unit.turnPowerOff();
			}
			else
			{
				unit.turnPowerOn();
			}
			minDuration = dTime; //set back to minDuration
		}
		else
		{
			minDuration--;
		}

	}
	outLogFile.close();
}

long getMillisec()
{
	chrono::milliseconds mstime = chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch());
	return mstime.count();
}

void displayStats(InputFile *iFile)
{
	cout << "Records in output: " << iFile->getCount() << endl;

	cout << "MaxTemp: " << iFile->getMaxBeerTemp() << endl;
	cout << "MinTemp: " << iFile->getMinBeerTemp() << endl;
	cout << "Max Outside Temp: " << iFile->getMaxOutsideTemp() << endl;
	cout << "Min Outside Temp: " << iFile->getMinOutsideTemp() << endl;
	cout << "Max Humidity: %" << iFile->getMaxHumidity() << endl;
	cout << "Min Humidity: %" << iFile->getMinHumidity() << endl;
	cout << "Max temp difference between outside and beer: "
			<< iFile->getMaxTempDiff() << endl;
	cout << "Max Set temperature was: " << iFile->getMaxSetTemp() << endl;
	cout << "Min Set temperature was: " << iFile->getMinSetTemp() << endl;
	cout << "Maximum the temp was off from set point: "
			<< iFile->getMaxTempOff() << endl;
	cout << "Average difference from set point: " << iFile->getAverateTempOff()
			<< endl;
}
