// Enter the milepost for the detector below
char MILEPOST[]="36.2";

// The distance between the first input and 
// the second input (in scale feet)
int DISTANCE = 80;

// Percent of trains that should trigger a defect.
int DEFECTPERCENT=10;

//Define the Pin Numbers used in the sketch.

// Active and inactive are aliases to high and low

#define DETECTORACTIVE LOW
#define DETECTORINACTIVE HIGH

// Ready
#define RDY  13

// SpeakJet Reset Pin
#define RES  3

// Speaking
#define SPK  4

//Transmit to SpeakJet
#define txPin  2

// Sensor Inputs for defect detector.
#define TRIGGER1PIN 6
#define TRIGGER2PIN 7
#define TRIGGER3PIN 8
#define TRIGGER4PIN 9

typedef struct __detector {
  char milepost[10];
  char track[10];
  int distance;
  int triggerPin1;
  int triggerPin2;
  float speed;
  int speedtrigger;
  int firstTime;
  int defect;
} detector;

void initDetector(detector *d,int pin1,int pin2);
int detectorActive(detector *d);
void resetDetectorState(detector d);
