#ifndef THECARLIBRARY_H
#define THECARLIBRARY_H

//States
#define START 0
#define HIGHWAY 1
#define PARKING 2
#define ROTATELEFT 3
#define ROTATERIGHT 4
#define CITY 5
#define FINSH 6
#define STOPLINE 7
#define OFF 8


#include "Servo.h"
#include <Arduino.h>



class TheCar
{
	
	public:
		//Variables
		byte _INA1 = 2;
		byte _INB1 = 4;
		byte _EN1DIAG1 = 5;
		byte _CS1 = A0;
		byte _PWM1 = 3;

		byte _INA2 = 7;
		byte _INB2 = 8;
		byte _EN2DIAG2 = 12;
		byte _CS2 = A1;
		byte _PWM2 = 11;

		byte leftEye = 5;
		byte frontEye = 4;
		byte rightEye = 2;
		byte colorEye = 7;

		byte stopline = 0;
		Servo serv;
		int leftDist, rightDist, fDist;
		int fDistPrev = 0;
		int U;
	
		
	
		float p = 0.3f;
		byte V0 = 50;
		bool tr = false;
		
		//functions
		void init();
		void setM1Speed(int sp);
		void setM2Speed(int sp);
		unsigned char getM1Fault();
		unsigned char getM2Fault();
		short getLeftEye();
		short getRightEye();
		short getFrontEye();
		void to_center();
		
		//Constructors
		TheCar();
		
	
};



#endif