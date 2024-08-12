#ifndef __Audio_h
#define __Audio_h

/* Speak Jet routines */
void speakJetReset();
void speakJetWait();
//Send "Hotbox detector, milepost" to the SpeakJet module
void readWelcome();
// read the mile post number, one digit at a time.
void readMilepost(char []);
void readNoDefects();
void readDetectorOut();
void readIntegretyFailure();
// Read the track
void readTrack(int track);
// read a single digit
void speakdigit(char digit);

//Read the speed in MPH.
void readspeed(float speed);
// Defect Alarm
void defect_alarm();

#endif