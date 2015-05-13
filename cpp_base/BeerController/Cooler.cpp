/*
 * Cooler.cpp
 *
 *  Created on: May 10, 2015
 *      Author: dhedley and msanders
 */

#include "Cooler.h"

Cooler::Cooler(double currentTemp, double upSpeed, double downSpeed, double maxTempVar)
{
	this->currentTemp = currentTemp;
	tpIncreaseSpeed = upSpeed;
	tpDecreaseSpeed = downSpeed;
	maxTempVariance = maxTempVar;
	power = false;

}

Cooler::~Cooler()
{
	// TODO Auto-generated destructor stub
}

void Cooler::turnPowerOn()
{
	power = true;
}

void Cooler::turnPowerOff()
{
	power = false;
}

double Cooler::getTemperature()
{
	if(power){
		currentTemp = currentTemp - tpDecreaseSpeed;
	} else {
		currentTemp = currentTemp + tpIncreaseSpeed;
	}

	return currentTemp;

}

bool Cooler::getPowerStatus(){
	return power;
}
