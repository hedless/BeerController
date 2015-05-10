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

InputFile::InputFile(string filePath) {
	inputFilePath = filePath;
	crunchFile();
}

InputFile::~InputFile() {

}

void InputFile::crunchFile(){
	string line;
	ifstream beerLogFile(inputFilePath);

	while (getline(beerLogFile, line)){

		line.c_str();
		recordCount++;

	}

	beerLogFile.close();
}

int InputFile::getCount(){
	return recordCount++;
}
