#include "Arduino.h"
#include "Audio.h"

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