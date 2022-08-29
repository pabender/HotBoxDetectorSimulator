// Enter the milepost for the detector below
char MILEPOST[]="36.2";

// The distance between the first input and 
// the second input (in scale feet)
int DISTANCE = 80;

// Percent of trains that should trigger a defect.
int DEFECTPERCENT=10;

//Define the Pin Numbers used in the sketch.

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
