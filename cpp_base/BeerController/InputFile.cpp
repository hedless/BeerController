/*
 * InputFile.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */

#include "InputFile.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

InputFile::InputFile(string filePath, string outputFilePath)
{
	inputFilePath = filePath;
	outFile = outputFilePath;
	countRecords();
	convertFile(); //convert the text file into a binary file.

	updateStats(); //update the min and max stats from dat file
}

InputFile::~InputFile()
{

}

void InputFile::countRecords()
{
	string line;
	ifstream beerLogFile(inputFilePath);
	int count = 0;
	while (getline(beerLogFile, line))
	{
		count++;
	}
	recordCount = count;
	beerLogFile.close();
}

void InputFile::convertFile()
{
	string line;
	ifstream beerLogFile(inputFilePath);

	BeerRecord * br = new BeerRecord[recordCount];

	int count = 0;
	while (getline(beerLogFile, line))
	{

		/* converting string to cstring before spliting it up into
		 * other parts for later.
		 */
		const char * cLine;
		cLine = line.c_str();

		int column = 0;
		int index = 0;
		int begIndex = 0;

		while (cLine[index] != '\0')
		{
			if (cLine[index] == ',')
			{
				if (column == 0) //the Date column
				{
					char tempCS[(index - begIndex) + 1];
					getSubCString(begIndex, index, cLine, tempCS);

					for (int i = 0; i < timeSize - 1; i++)
						br[count].time[i] = tempCS[i]; // copy date info into cstring
				}
				else if (column == 1)
				{
					char tempCS[(index - begIndex) + 1];
					getSubCString(begIndex, index, cLine, tempCS); //grab substring from cstring
					br[count].internalTemp = atof(tempCS); //convert to double
				}
				else if (column == 2)
				{
					char tempCS[(index - begIndex) + 1];
					getSubCString(begIndex, index, cLine, tempCS); //grab substring from cstring
					br[count].externalTemp = atof(tempCS); //convert to double
				}
				else if (column == 3)
				{
					char tempCS[(index - begIndex) + 1];
					getSubCString(begIndex, index, cLine, tempCS); //grab substring from cstring
					br[count].setTemp = atoi(tempCS); //convert to int
				}
				else if (column == 4)
				{
					char tempCS[(index - begIndex) + 1];
					getSubCString(begIndex, index, cLine, tempCS); //grab substring from cstring
					br[count].humidity = atof(tempCS); //convert to double
				}
				else if (column == 5)
				{
					char tempCS[(index - begIndex) + 1];

					getSubCString(begIndex, index, cLine, tempCS); //grab substring from cstring
					string tempST = tempCS;

					if (tempST == "On")
						br[count].powerSwitch = true;
					else
						br[count].powerSwitch = false;

					//now grab the last column since we are out of commas
					char tempCSlast[line.length() - index];
					getSubCString(index + 1, line.length(), cLine, tempCSlast);
					br[count].numberVar = atof(tempCSlast);

				}

				begIndex = index + 1; //set begining index to after the comma
				column++; //move to next column.

			}
			index++;

		}

//		cout << cLine << endl;
//
//		cout << "Date: " << br[count].time << endl;
//		cout << "internal temp: " << br[count].internalTemp << endl;
//		cout << "out temp: " << br[count].externalTemp << endl;
//		cout << "set temp: " << br[count].setTemp << endl;
//		cout << "humidity: " << br[count].humidity << endl;
//		cout << "power: " << br[count].powerSwitch << endl;
//		cout << "numbar val " << br[count].numberVar << endl;

		count++;
	}

	beerLogFile.close();

	saveToFile(br);

}

int InputFile::getCount()
{
	return recordCount++;
}

void InputFile::getSubCString(int beg, int end, const char *cstring,
		char * substring)
{
	for (int i = beg; i < end; i++)
	{
		substring[i - beg] = cstring[i];
	}

	substring[(end - beg)] = '\0';
	//cout << "substring: " << substring << " beg: " << beg << "end: " << end << endl;
}

void InputFile::saveToFile(BeerRecord *bArray)
{
	fstream outputFile;
	outputFile.open(outFile, ios::out | ios::binary);

	for (int i = 0; i < recordCount; i++)
	{
		outputFile.write(reinterpret_cast<char *>(&bArray[i]),
				sizeof(bArray[i]));
	}
	outputFile.close();
}

void InputFile::updateStats()
{
	BeerRecord br;

	fstream datFile;
	datFile.open(outFile, ios::in | ios::binary);

	if (!datFile)
	{
		cout << "Error opening dat file: " << outFile << endl;
	}

	datFile.read(reinterpret_cast<char *>(&br), sizeof(BeerRecord));

	maxBeerTemp = br.internalTemp;
	minBeerTemp = br.internalTemp;
	maxOutTemp = br.externalTemp;
	minOutTemp = br.externalTemp;
	maxHumidity = br.humidity;
	minHumidity = br.humidity;
	maxTempDiff = br.externalTemp - br.internalTemp;
	maxSetTemp = br.setTemp;
	minSetTemp = br.setTemp;
	if(br.internalTemp > br.setTemp)
		maxTempOff = br.internalTemp - br.setTemp;
	else
		maxTempOff = br.setTemp - br.internalTemp;

	double sumMaxTempOff = maxTempOff;

	while (!datFile.eof())
	{
		if (br.internalTemp > maxBeerTemp)
			maxBeerTemp = br.internalTemp;
		if (br.internalTemp < minBeerTemp)
			minBeerTemp = br.internalTemp;
		if (br.externalTemp > maxOutTemp)
			maxOutTemp = br.externalTemp;
		if (br.externalTemp < minOutTemp)
			minOutTemp = br.externalTemp;
		if (br.humidity > maxHumidity)
			maxHumidity = br.humidity;
		if (br.humidity < minHumidity)
			minHumidity = br.humidity;
		if (maxTempDiff < br.externalTemp - br.internalTemp)
			maxTempDiff = br.externalTemp - br.internalTemp;
		if (maxSetTemp < br.setTemp)
			maxSetTemp = br.setTemp;
		if (minSetTemp > br.setTemp)
			minSetTemp = br.setTemp;

		//find max amount that temp was different than set point regardless if it was above or below
		double tempMaxTempOff;
		if(br.internalTemp > br.setTemp)
			tempMaxTempOff = br.internalTemp - br.setTemp;
		else
			tempMaxTempOff = br.setTemp - br.internalTemp;
		if (maxTempOff < tempMaxTempOff)
			maxTempOff = tempMaxTempOff;
		sumMaxTempOff = sumMaxTempOff + tempMaxTempOff; //to calculate average temperature off set

		datFile.read(reinterpret_cast<char *>(&br), sizeof(BeerRecord));
	}

	averageTempOff = (sumMaxTempOff/recordCount) * 1.0; //output as double

	datFile.close();
}

double InputFile::getMaxBeerTemp()
{
	return maxBeerTemp;
}

double InputFile::getMinBeerTemp()
{
	return minBeerTemp;
}

double InputFile::getMaxOutsideTemp()
{
	return maxOutTemp;
}

double InputFile::getMinOutsideTemp()
{
	return minOutTemp;
}

double InputFile::getMaxHumidity()
{
	return maxHumidity;
}

double InputFile::getMinHumidity()
{
	return minHumidity;
}

double InputFile::getMaxTempDiff()
{
	return maxTempDiff;
}

int InputFile::getMaxSetTemp()
{
	return maxSetTemp;
}

int InputFile::getMinSetTemp()
{
	return minSetTemp;
}

double InputFile::getMaxTempOff()
{
	return maxTempOff;
}

double InputFile::getAverateTempOff()
{
	return averageTempOff;
}
