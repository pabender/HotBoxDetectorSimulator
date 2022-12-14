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
#include "Audio.h"
#include "Detector.h"
#include "Messages.h"
#include "Numbers.h"

//Create a SoftSerial Objet
SoftwareSerial speakjet = SoftwareSerial(0, txPin);

detector track1Detector;
  
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

  initDetector(&track1Detector,TRIGGER1PIN,TRIGGER2PIN);     
  speakJetReset();
}

void loop() { 

  while(!detectorActive(track1Detector)); // while both triggers are inactive, wait

  resetDetectorState(&track1Detector);
   
  readWelcome();
 

  //Wait before sending the next string.
  while(digitalRead(SPK)!=LOW){
    if(digitalRead(track1Detector.speedtrigger)== DETECTORACTIVE && track1Detector.speed==0){
            track1Detector.speed = calcSpeed(track1Detector.firstTime,millis());
    }
  }
  
  readMilepost(track1Detector.milepost);
  
  // Now wait until the detector input changes
  while(detectorActive(track1Detector)){
          if(digitalRead(track1Detector.speedtrigger)==DETECTORACTIVE && track1Detector.speed==0){
            track1Detector.speed=calcSpeed(track1Detector.firstTime,millis());
          }
          if(track1Detector.defect<=DEFECTPERCENT) {
             // we have already determined if we have a deffect, now 
             // we need to decide if this is where we want to trigger
             // the alarm.
             if(random(100)<=DEFECTPERCENT) break;
          }
          
        } // while either trigger is HIGH, wait
        
  if(track1Detector.defect<=(DEFECTPERCENT)){
    defect_alarm();
    // after triggering the defect alarm, wait until the
    // trigger pins are both low again before continuing.
    while(detectorActive(track1Detector));
  } else {
    //Send "Hotbox detector, milepost" to the SpeakJet module
    readWelcome();
    readMilepost(track1Detector.milepost);
  
    if(track1Detector.speed!=0){
      readspeed(track1Detector.speed);
    }

    readNoDefects();
    readDetectorOut();
  }
}

/* Speak Jet routines */
void speakJetReset(){
  //Reset the SpeakJet module
  digitalWrite(RES, LOW);
  delay(100);
  digitalWrite(RES, HIGH);
}

void speakJetWait(){
    while(digitalRead(SPK)!=LOW);  
}

//Send "Hotbox detector, milepost" to the SpeakJet module
void readWelcome(){  
  speakJetWait();
  speakjet.print(welcome);
}

// read the mile post number, one digit at a time.
void readMilepost(char milepost[])
{
  speakJetWait();
  int i;
  for(i=0;i<strlen(milepost);i++){
     speakdigit(milepost[i]);    
  }
}

void readNoDefects(){
    speakJetWait();
    speakjet.print(no_defects);
}

void readDetectorOut(){
    speakJetWait();
    speakjet.print(detector_out);
}



// read a single digit
void speakdigit(char digit)
{
  speakJetWait();
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

//Calculate the speed
float calcSpeed(int first,int last){
    if(first==last) 
       return 0;
    else
       return (DISTANCE/5280.0)/((last-first)/3600000.0);
}

//Read the speed in MPH.
void readspeed(float speed)
{
  speakJetWait();
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
     speakJetWait();
     speakdigit(speedstring[i]);    
  }
}

// Defect Alarm
void defect_alarm(){
  speakJetWait();
  speakjet.print(alarm_string);
  speakJetWait();
  speakjet.print(alarm_string);
}


/* detector routines */
void initDetector(detector *d,int pin1,int pin2){
  int i;
  for(i=0;i<strlen(MILEPOST);i++){
    (*d).milepost[i]=MILEPOST[i];
  }
  (*d).milepost[i]='\0';
  (*d).speed = 0;
  (*d).triggerPin1=pin1;
  (*d).triggerPin2=pin2;
  (*d).speedtrigger=(*d).triggerPin2;
  (*d).firstTime=0;

  pinMode((*d).triggerPin1,INPUT);
  pinMode((*d).triggerPin2,INPUT);

}

int detectorActive(detector d){
  return (digitalRead(d.triggerPin1)==DETECTORACTIVE
        || digitalRead(d.triggerPin2)==DETECTORACTIVE);
}

void resetDetectorState(detector *d){
  if(digitalRead((*d).triggerPin1)==DETECTORACTIVE){
     (*d).speedtrigger=(*d).triggerPin2;  
  } else  {
     (*d).speedtrigger=(*d).triggerPin1;
  }
  (*d).firstTime=millis();
  randomSeed((*d).firstTime);
  (*d).defect=random(100);
  (*d).speed = 0;
}
