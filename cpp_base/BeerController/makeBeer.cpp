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



}

