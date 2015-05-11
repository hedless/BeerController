/*
 * makeBeer.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */

#include <iostream>
#include <fstream>
#include <string>
#include "InputFile.h"
using namespace std;

int main()
{

	int count = 0;

	string inputPath = "/Users/dhedley/BeerController/sensor_output";
	string inputFileName = "beerlog.txt";
	string outputFileName = "outputBeerLog.dat";

	//date,beer temp, outside temp, set temp, humidity, power state, PINout
	//2015-01-04 20:56:19,67.21,73.40,67,36.00,Off,4748.09

	//Create object of the beer log
	InputFile *beerLog;
	beerLog = new InputFile(inputPath + "/" + inputFileName, inputPath + "/" + outputFileName);

	//output number of records found in the log file
	count = beerLog->getCount();

	cout << "Records in output: " << count << endl;

	cout << "MaxTemp: " << beerLog->getMaxBeerTemp() << endl;
	cout << "MinTemp: " << beerLog->getMinBeerTemp() << endl;
	cout << "Max Outside Temp: " << beerLog->getMaxOutsideTemp() << endl;
	cout << "Min Outside Temp: " << beerLog->getMinOutsideTemp() << endl;
	cout << "Max Humidity: %" << beerLog->getMaxHumidity() << endl;
	cout << "Min Humidity: %" << beerLog->getMinHumidity() << endl;
	cout << "Max temp difference between outside and beer: " << beerLog->getMaxTempDiff() << endl;
	cout << "Max Set temperature was: " << beerLog->getMaxSetTemp() << endl;
	cout << "Min Set temperature was: " << beerLog->getMinSetTemp() << endl;
	cout << "Maximum the temp was off from set point: " << beerLog->getMaxTempOff() << endl;
	cout << "Average difference from set point: " << beerLog->getAverateTempOff() << endl;


}

