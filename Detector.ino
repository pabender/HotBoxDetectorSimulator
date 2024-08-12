#include "Arduino.h"
#include "Detector.h"
#include "Audio.h"

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
  (*d).defect=random()%100;
  (*d).speed = 0;
}
