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
	convertFile();
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
	while (getline(beerLogFile, line) && count < 10)
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

		cout << cLine << endl;

		cout << "Date: " << br[count].time << endl;
		cout << "internal temp: " << br[count].internalTemp << endl;
		cout << "out temp: " << br[count].externalTemp << endl;
		cout << "set temp: " << br[count].setTemp << endl;
		cout << "humidity: " << br[count].humidity << endl;
		cout << "power: " << br[count].powerSwitch << endl;
		cout << "numbar val " << br[count].numberVar << endl;

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

	for(int i = 0; i < recordCount; i++){
		outputFile.write(reinterpret_cast<char *>(&bArray[i]),sizeof(bArray[i]));
	}
}



//		char * cLine = new char[line.length() + 1]; //create cstring array
//		strcpy(cLine,line.c_str()); //copy string into the cstring
//		std::string str = "string";
//		char *cstr = new char[str.length() + 1];
//		strcpy(cstr, str.c_str());
//		// do stuff
//		delete [] cstr;
