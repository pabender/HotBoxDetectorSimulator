/*

Defect Detector Simulator

Based on the Voice Box Demo Sketch
Written by Ryan Owens
SparkFun Electronics

modified by Paul Bender paul.bender@acm.org

Uses the Voice Box Shield from SparkFun to 
simulate a railroad defect detector.

A speaker can be plugged directly into 
the SPK+ and - pins on the shield.

*/

// Soft serial library used to send commands 
// on pin 2 instead of regular serial pin.
#include <SoftwareSerial.h>
#include "Detector.h"
#include "Messages.h"
#include "Numbers.h"

//Create a SoftSerial Objet
SoftwareSerial speakjet = SoftwareSerial(0, txPin);

void setup()  
{
  //Configure the pins for the SpeakJet module
  pinMode(txPin, OUTPUT);
  pinMode(SPK, INPUT);
  
  //Set up a serial port to talk from Arduino to the SpeakJet module on pin 3.
  speakjet.begin(9600);    
  
  //Configure the Ready pin as an input
  pinMode(RDY, INPUT);
  
  //Configure Reset line as an output
  pinMode(RES, OUTPUT);
       
  pinMode(TRIGGER1PIN,INPUT);
  pinMode(TRIGGER2PIN,INPUT);

  //Reset the SpeakJet module
  digitalWrite(RES, LOW);
  delay(100);
  digitalWrite(RES, HIGH);
  
}

void loop()
{ 
  int speedtrigger=TRIGGER2PIN;
  int first=0,last=0;
  unsigned int defect=0;
  float speed=0;
  
  while(digitalRead(TRIGGER1PIN)==HIGH
        && digitalRead(TRIGGER2PIN)==HIGH); // while both triggers are low, wait
  if(digitalRead(TRIGGER1PIN)==LOW){
     speedtrigger=TRIGGER2PIN;
     first=millis();
     randomSeed(first);
     defect=random(100);
     speed = 0;
     last=0;
  } else  {
     speedtrigger=TRIGGER1PIN;
     first=millis();
     randomSeed(first);
     defect=random(100);
     speed = 0;
     last=0;
  }
  

  //Send "Hotbox detector, milepost" to the SpeakJet module
  speakjet.print(welcome);
  //Wait before sending the next string.
  while(digitalRead(SPK)!=LOW){
    if(digitalRead(speedtrigger)==LOW && speed==0){
            last = millis();
            if(first==last) 
              speed = 0;
            else
              speed = (DISTANCE/5280.0)/((last-first)/3600000.0);
    }
  }
  milepost();
  
  // Now wait until the detector input changes
  while(digitalRead(TRIGGER1PIN)==LOW 
        || digitalRead(TRIGGER2PIN)==LOW){
          if(digitalRead(speedtrigger)==LOW && speed==0){
            last = millis();
            if(first==last) 
              speed = 0;
            else
              speed = (DISTANCE/5280.0)/((last-first)/3600000.0);
          }
          if(defect<=DEFECTPERCENT) {
             // we have already determined if we have a deffect, now 
             // we need to decide if this is where we want to trigger
             // the alarm.
             if(random(100)<=DEFECTPERCENT) break;
          }
          
        } // while either trigger is HIGH, wait
        
  if(defect<=(DEFECTPERCENT)){
    defect_alarm();
    // after triggering the defect alarm, wait until the
    // trigger pins are both low again before continuing.
    while(digitalRead(TRIGGER1PIN)==LOW 
        || digitalRead(TRIGGER2PIN)==LOW);
  } else {
    //Send "Hotbox detector, milepost" to the SpeakJet module
    speakjet.print(welcome);
    //Wait before sending the next string.
    while(digitalRead(SPK)!=LOW);
    milepost();
    //Wait before sending the next string.
    while(digitalRead(SPK)!=LOW);
    if(speed!=0){
      readspeed(speed);
    }
    while(digitalRead(SPK)!=LOW);
    speakjet.print(no_defects);

    while(digitalRead(SPK)!=LOW);
    speakjet.print(detector_out);
  }
}


// read the mile post number, one digit at a time.
void milepost()
{
  int i;
  for(i=0;i<strlen(MILEPOST);i++){
     while(digitalRead(SPK)!=LOW);
     speakdigit(MILEPOST[i]);    
  }
}

// read a single digit
void speakdigit(char digit)
{
  while(digitalRead(SPK)!=LOW);
  switch(digit)
  {
    case '1':
           speakjet.print(ONE);
           break;
    case '2':
      speakjet.print(TWO);
           break;
    case '3':
      speakjet.print(THREE);
           break;
    case '4':
      speakjet.print(FOUR);
           break;
    case '5':
      speakjet.print(FIVE);
           break;
    case '6':
      speakjet.print(SIX);
           break;
    case '7':
      speakjet.print(SEVEN);
           break;
    case '8':
      speakjet.print(EIGHT);
      break;
    case '9':
      speakjet.print(NINE);
           break;
    case '0':
      speakjet.print(ZERO);
           break;
    case '.':
      speakjet.print(DOT);
           break;
  }
  
}

//Read the speed in MPH.
void readspeed(float speed)
{
  char speedstring[100];
  int speedstep=1;
  int i=0;

  // create a string with the speed by division.
  // the speed ends up in the string backwards.
  while(speed>0){
    speedstring[i]=(int)speed%10+'0';
    speed=(int)speed/10;
    i++;
  }
  speedstring[i]='\0';
  speakjet.print(speed_string);
  
  // Since the string is backwards, 
  // start at the last character, and 
  // work our way to the begining.
  for(i=strlen(speedstring);i>=0;i--){
     while(digitalRead(SPK)!=LOW);
     speakdigit(speedstring[i]);    
  }
}

// Defect Alarm
void defect_alarm()
{
  speakjet.print(alarm_string);
  while(digitalRead(SPK)!=LOW);
  speakjet.print(alarm_string);
}
