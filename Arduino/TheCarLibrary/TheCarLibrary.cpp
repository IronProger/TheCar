#include "TheCarLibrary.h"
#include <Arduino.h>

TheCar::TheCar()
{
	
}

void TheCar::init()
{
	pinMode(colorEye, INPUT);
	pinMode(_INA1, OUTPUT);
	pinMode(_INB1, OUTPUT);
	pinMode(_PWM1, OUTPUT);
	pinMode(_EN1DIAG1, INPUT_PULLUP);
	pinMode(_CS1, INPUT);
	pinMode(_INA2, OUTPUT);
	pinMode(_INB2, OUTPUT);
	pinMode(_PWM2, OUTPUT);
	pinMode(_EN2DIAG2, INPUT_PULLUP);
	pinMode(_CS2, INPUT);
	serv.attach(12);
  	serv.write(45);
}

	
void TheCar::setM1Speed(int sp) {
	sp = constrain(sp, -255, 255);
	if (sp < 0) {
		digitalWrite(_INA1, LOW);
		digitalWrite(_INB1, HIGH);
		sp *= -1;
	} 
	else if (sp > 0) {
		digitalWrite(_INA1, HIGH);
		digitalWrite(_INB1, LOW);
	} 
	else {
		digitalWrite(_INA1, LOW);
		digitalWrite(_INB1, LOW);
	}
	analogWrite(_PWM1, sp);
}

void TheCar::setM2Speed(int sp) {
	sp = constrain(sp, -255, 255);
	if (sp < 0) {
		digitalWrite(_INA2, LOW);
		digitalWrite(_INB2, HIGH);
		sp *= -1;
	} 
	else if (sp > 0) {
		digitalWrite(_INA2, HIGH);
		digitalWrite(_INB2, LOW);
	} 
	else {
		digitalWrite(_INA2, LOW);
		digitalWrite(_INB2, LOW);
	}
	analogWrite(_PWM2, sp);
}

unsigned char TheCar::getM1Fault()
{
	return !digitalRead(_EN1DIAG1);
}

unsigned char TheCar::getM2Fault()
{
	return !digitalRead(_EN2DIAG2);
}

short TheCar::getLeftEye()
{
	return (short)analogRead(leftEye);	
}

short TheCar::getRightEye()
{
	return (short)analogRead(rightEye);
}

short TheCar::getFrontEye()
{
	return (short)analogRead(frontEye);
}

void TheCar::to_center()
{
	leftDist = getLeftEye();
	rightDist = getRightEye();
	U = p * (leftDist - rightDist);
	U = constrain(U, -40, 40);
	serv.write(45 - U);
}