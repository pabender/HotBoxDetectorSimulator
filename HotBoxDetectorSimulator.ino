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


//Calculate the speed
float calcSpeed(int first,int last){
    if(first==last) 
       return 0;
    else
       return (DISTANCE/5280.0)/((last-first)/3600000.0);
}
  
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

