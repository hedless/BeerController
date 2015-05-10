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
		char time[19];
		double internalTemp;
		double externalTemp;
		int setTemp;
		double humidity;
		bool powerSwitch;
		double numberVar;
	};
	const int timeSize = 19;
	string inputFilePath;
	void countRecords(); //grab the count of the file lines
	void crunchFile(); //load file stats into class object
	void getSubCString(int beg, int end, const char *cstring, char *substring);
	int recordCount;

public:
	InputFile(string filePath);
	virtual ~InputFile();
	int getCount(); //get the number of records in file
};

#endif /* INPUTFILE_H_ */
