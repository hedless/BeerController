/*
 * InputFile.h
 *
 *  Created on: May 1, 2015
 *      Author: dhedley and msanders
 */

#ifndef INPUTFILE_H_
#define INPUTFILE_H_

#include<string>
using namespace std;

class InputFile
{
private:
	struct BeerRecord
	{
		char time[20];
		double internalTemp;
		double externalTemp;
		int setTemp;
		double humidity;
		bool powerSwitch;
		double numberVar;
	};

	int timeSize;
	string inputFilePath;
	int recordCount;
	string outFile;
	double maxBeerTemp;
	double maxOutTemp;
	double minBeerTemp;
	double minOutTemp;
	double maxHumidity;
	double minHumidity;
	double maxTempDiff;
	double maxTempOff;
	double averageTempOff;
	int maxSetTemp;
	int minSetTemp;

	void countRecords(); //grab the count of the file lines
	void convertFile(); //load file stats into class object
	void getSubCString(int, int, const char *, char *);
	void saveToFile(BeerRecord *); //save the input data to a binary dat file


public:
	InputFile(string , string);
	virtual ~InputFile();
	int getCount(); //get the number of records in file
	double getMaxBeerTemp();
	double getMinBeerTemp();
	double getMaxOutsideTemp();
	double getMinOutsideTemp();
	double getMaxHumidity();
	double getMinHumidity();
	double getMaxTempDiff();
	double getMaxTimeOn();
	double getMaxTimeOff();
	double getMaxTempOff();
	double getAverateTempOff();
	int getMaxSetTemp();
	int getMinSetTemp();
	void updateStats();
};

#endif /* INPUTFILE_H_ */
