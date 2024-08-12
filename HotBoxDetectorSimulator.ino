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

// define the DOUBLETRACK constant if two tracks are in use.
//#define DOUBLETRACK 1

//Create a SoftSerial Objet
SoftwareSerial speakjet = SoftwareSerial(0, txPin);

detector track1Detector;
#ifdef DOUBLETRACK 
detector track2Detector;
#endif

#define debounceDelay 2000

  
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
  speakJetReset();

  initDetector(&track1Detector,1,TRIGGER1PIN,TRIGGER2PIN);
#ifdef DOUBLETRACK  
  initDetector(&track2Detector,2,TRIGGER3PIN,TRIGGER4PIN);
#endif  
  offline(track1Detector);
  while(detectorActive(&track1Detector) 
#ifdef DOUBLETRACK
        || detectorActive(&track2Detector)
#endif
        ); // while any trigger is active, wait

}

void loop() { 

  while(!detectorActive(&track1Detector) 
#ifdef DOUBLETRACK
        && !detectorActive(&track2Detector)
#endif
        ); // while both triggers are inactive, wait

  if(detectorActive(&track1Detector)) { // the detector on track 1 is active

     resetDetectorState(&track1Detector);
     startDetector(&track1Detector); 

     // Now wait until the detector input changes
     while(detectorActive(&track1Detector)){
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
       while(detectorActive(&track1Detector));
     } else {
       closing(track1Detector);
     }
  }
  
#ifdef DOUBLETRACK  
  if(detectorActive(&track2Detector)) { // the detector on track 2 is active

     resetDetectorState(&track2Detector);
     startDetector(&track2Detector); 

     // Now wait until the detector input changes
     while(detectorActive(&track2Detector)){
          if(digitalRead(track2Detector.speedtrigger)==DETECTORACTIVE && track2Detector.speed==0){
            track2Detector.speed=calcSpeed(track2Detector.firstTime,millis());
          }
          if(track2Detector.defect<=DEFECTPERCENT) {
             // we have already determined if we have a deffect, now 
             // we need to decide if this is where we want to trigger
             // the alarm.
             if(random(100)<=DEFECTPERCENT) break;
          }
          
     } // while either trigger is HIGH, wait
        
     if(track2Detector.defect<=(DEFECTPERCENT)){
       defect_alarm();
       // after triggering the defect alarm, wait until the
       // trigger pins are both low again before continuing.
       while(detectorActive(&track2Detector));
     } else {
       closing(track2Detector);
     }
  }
#endif

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

// read the mile post number, one digit at a time.
void readTrack(char track[])
{
  speakJetWait();
  speakjet.print(track_string);
  speakJetWait();
  int i;
  for(i=0;i<strlen(track);i++){
     speakdigit(track[i]);    
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

void readIntegretyFailure(){
    speakJetWait();
    speakjet.print(integrety_failure);
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
void initDetector(detector *d,int track,int pin1,int pin2){
  int i;
  for(i=0;i<strlen(MILEPOST);i++){
    (*d).milepost[i]=MILEPOST[i];
  }
  (*d).milepost[i]='\0';
  (*d).track[0]='0'+track;
  (*d).track[1]='\0';
  (*d).speed = 0;
  (*d).triggerPin1=pin1;
  (*d).triggerPin2=pin2;
  (*d).speedtrigger=(*d).triggerPin2;
  (*d).firstTime=0;
  (*d).activeState=DETECTORINACTIVE;
  (*d).debounceState=DETECTORINACTIVE;
  (*d).lastDebounceTime=millis();

  pinMode((*d).triggerPin1,INPUT);
  pinMode((*d).triggerPin2,INPUT);

}

void startDetector(detector *d){
    readWelcome();
 

  //Wait before sending the next string.
  while(digitalRead(SPK)!=LOW){
    if(digitalRead((*d).speedtrigger)== DETECTORACTIVE && (*d).speed==0){
            (*d).speed = calcSpeed((*d).firstTime,millis());
    }
  }
  
  readMilepost((*d).milepost);
#ifdef DOUBLETRACK
  readTrack((*d).track);
#endif  
}

void closing(detector d){

    //Send "Hotbox detector, milepost" to the SpeakJet module
    readWelcome();
    readMilepost(d.milepost);
#ifdef DOUBLETRACK
    readTrack(d.track);
#endif  

    if(d.speed!=0){
      readspeed(d.speed);
    }

    readNoDefects();
    readDetectorOut();
}

void offline(detector d){

    //Send "Hotbox detector, milepost" to the SpeakJet module
    readWelcome();
    readMilepost(d.milepost);
    readIntegretyFailure();
    readDetectorOut();
}


int detectorActive(detector *d){
  
  int state = (digitalRead((*d).triggerPin1)==DETECTORACTIVE
        || digitalRead((*d).triggerPin2)==DETECTORACTIVE);

  if((*d).debounceState != state ){
    (*d).lastDebounceTime = millis();
    (*d).debounceState = state;
  }
        
  if(millis()-(*d).lastDebounceTime > debounceDelay){
     (*d).activeState=(*d).debounceState;      
  }
  return ((*d).activeState);
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
