/*
 * Cooler.h
 *
 *  Created on: May 10, 2015
 *      Author: dhedley
 */

#ifndef COOLER_H_
#define COOLER_H_

class Cooler
{
private:
	double currentTemp;
	double outsideTemp;
	int time;
	int lastTime;
	double maxTempVariance; //max temperature difference between outside and inside temps
	double tpIncreaseSpeed;
	double tpDecreaseSpeed;
	bool power = false;

public:
	Cooler(double, double, double, double);
	virtual ~Cooler();
	void turnPowerOn();
	void turnPowerOff();
	double getTemperature();
	bool getPowerStatus();
};

#endif /* COOLER_H_ */
