/* Speak Jet routines */
void speakJetReset();
void speakJetWait();
//Send "Hotbox detector, milepost" to the SpeakJet module
void readWelcome();
// read the mile post number, one digit at a time.
void readMilepost(char []);
void readNoDefects();
void readDetectorOut();
// Read the track
void readTrack(int track);
// read a single digit
void speakdigit(char digit);
//Calculate the speed
float calcSpeed(int first,int last);
//Read the speed in MPH.
void readspeed(float speed);
// Defect Alarm
void defect_alarm();
