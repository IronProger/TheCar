#include "Servo.h"

#include "TheCarLibrary.h"

unsigned long int T;

byte lol;

TheCar car;

Servo serv;

unsigned long long StopTime = 0;

void setup() {
  StopTime--;
  car.init();
  T = 0;
  pinMode(9, INPUT_PULLUP);
  Serial.begin(9600);
  
  while (digitalRead(9)) {
    delay(10);
  }

  if (car.getM1Fault()) {
    Serial.println("M1 fault");
    while (1);
  } 
  else {
    Serial.println("OK");
  }
  
  


}

short State = START;
short LastState = START;


void loop() {
  if(millis()>=StopTime ) State = OFF;
  switch(State)
  {
    case START:
   {
     //TODO INIT
     car.setM1Speed(car.V0);
     StopTime=millis()+4*60*1000;
     LastState=State;
     State=HIGHWAY;
     
     break;
   }
   case HIGHWAY:
   {
     if(Serial.read() != -1)
     
     break;
   }
   case PARKING:
   {
     
     break;
   }
   case ROTATELEFT:
   {
     
     break;
   }
   case ROTATERIGHT:
   {
     
     break;
   }
   case CITY:
   {

     break;
   }
   case STOPLINE:
   {

     break;
   }
   case FINSH:
   {

     break;
   }
   case OFF:
   { 
     car.setM1Speed(0);
     while(1);
     break;
   }
   
  }
  
}
