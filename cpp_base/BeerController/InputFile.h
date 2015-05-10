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


class InputFile {
private:
	string inputFilePath;
	void crunchFile(); //load file stats into class object
	int recordCount;

public:
	InputFile(string filePath);
	virtual ~InputFile();
	int getCount(); //get the number of records in file
};

#endif /* INPUTFILE_H_ */
