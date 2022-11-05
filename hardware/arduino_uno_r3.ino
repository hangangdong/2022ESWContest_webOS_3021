//#include <Stepper.h>
#include <Unistep2.h>

// Pin configuration
#define HEATERPIN 2
#define FILTERPIN 3
#define LEDPIN 4
#define LEDGND 7
#define CDSGND 12
#define CDS5V 13

// Device number
#define FILTER 10
#define LIGHTING 20
#define HEATER 30
#define FEEDER 40

// Stepper configuration
int lap = 2048;
//Stepper stepper(lap, 8, 10, 9, 11);
int speed = 16;
int p1 = 8;
int p2 = 10;
int p3 = 9;
int p4 = 11;
int stepsPerRev = lap;
int stepDelay = 900;
Unistep2 stepperX(p1, p2, p3, p4, stepsPerRev, stepDelay);
  
// Illuminance  configuration
#define CDSPIN A0
int cds = 0;
int luxThreshold = 256;

void setup() {
  Serial.begin(115200);

  // Init pins
  pinMode(HEATERPIN, OUTPUT);
  pinMode(FILTERPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDPIN + 1, OUTPUT);
  pinMode(LEDPIN + 2, OUTPUT);
  pinMode(LEDGND, OUTPUT);
  pinMode(CDSGND, OUTPUT);
  pinMode(CDS5V, OUTPUT);
  digitalWrite(LEDGND, LOW);
  digitalWrite(CDSGND, LOW);
  digitalWrite(CDS5V, HIGH);

  // Init stepper
  //stepper.setSpeed(speed);

}

void loop() {
 
  if (Serial.available()) {
    // Read serial data
    int message = Serial.read();
    int device = message / 10 * 10;
    int value = message % 10;

    // Control output device
    switch (device) {
    case FILTER:
      digitalWrite(FILTERPIN, value);
      break;
    case LIGHTING:
      for (int i = 0; i < value; i++) {
        digitalWrite(LEDPIN + i, HIGH);
      }
      for (int i = value; i < 3; i++) {
        digitalWrite(LEDPIN + i, LOW);
      }
      break;
    case HEATER:
      digitalWrite(HEATERPIN, value);
      break;
    case FEEDER: 
      stepperX.moveTo(0);
      stepperX.move(1);
      if (stepperX.currentPosition() == 4095){
        stepperX.stop();
        }
      else{
        stepperX.run();
        } 
      //stepper.step(lap);
      break;
    }

    // Write serial data
    cds = analogRead(CDSPIN);
    Serial.write(cds / 10);
  }
}
