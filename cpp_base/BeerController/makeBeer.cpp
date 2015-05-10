/*
 * makeBeer.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {

	string line;
	int count = 0;
	string inputPath = "/Users/dhedley/BeerController/sensor_output";
	string inputFileName = "beerlog.txt";
	ifstream beerLogFile(inputPath + "/" + inputFileName);

	while (getline(beerLogFile, line)){

		//cout << line << endl;
		count++;

	}

	cout << "Records in output: " << count << endl;

	beerLogFile.close();
}

