/*
 * InputFile.h
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
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
	const int timeSize = 20;
	string inputFilePath;
	int recordCount;
	string outFile;
	void countRecords(); //grab the count of the file lines
	void convertFile(); //load file stats into class object
	void getSubCString(int, int, const char *, char *);
	void saveToFile(BeerRecord *); //save the input data to a binary dat file


public:
	InputFile(string , string);
	virtual ~InputFile();
	int getCount(); //get the number of records in file

};

#endif /* INPUTFILE_H_ */
