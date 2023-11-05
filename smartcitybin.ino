#include <LedControl.h>
#define TRIGPIN_01 1
#define ECHOPIN_01 2
#define TRIGPIN_02 3
#define ECHOPIN_02 4
#define PULPIN 7
#define DIRPIN 8
#define LED_DATA 9
#define LED_CS 10
#define LED_CLK 11
#define NUM_DEVICES 1
const int stepPerRevolution = 3200;
int y = 0;
int duration = 0;
int distance = 80;
int emptiness = 0;

LedControl lc = LedControl(LED_DATA,LED_CLK,LED_CS,NUM_DEVICES);
void setup() {
  pinMode(TRIGPIN_01,OUTPUT);
  pinMode(ECHOPIN_01,INPUT);
  pinMode(TRIGPIN_02,OUTPUT);
  pinMode(ECHOPIN_02,INPUT);
  pinMode(PULPIN,OUTPUT);
  pinMode(DIRPIN,OUTPUT);
  digitalWrite(DIRPIN,HIGH);
  Serial.begin(9600);
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
}

void runThreeQuarter(){
  int threequarter = (stepPerRevolution)/4;
  for (int i=0;i<threequarter;i++){
    digitalWrite(PULPIN,HIGH);
    digitalWrite(PULPIN,LOW);
    delayMicroseconds(1000);
  }
}

int grtDistance(int t,int e){
  digitalWrite(t,LOW);
  delayMicroseconds(2);
  digitalWrite(t,HIGH);
  delayMicroseconds(10);
  digitalWrite(t,LOW);
  int tduration = pulseIn(e,HIGH);
  int tdistance = tduration*0.034/2;
  return tdistance;
}

void turnOneRow(int row){
  for(int col = 0;col<8;col++){
    lc.setLed(0,row,col,true);
  }
}

void binStatus(int emptiness){
  lc.clearDisplay(0);
    int fullness=64-emptiness;
    int fullrows = (int)fullness/8;
    int fullsec = (int)fullness%8;
    for (int i = 0;i<fullrows;i++){
      turnOneRow(i);
    } 
}
void loop() {
  distance = grtDistance(TRIGPIN_02,ECHOPIN_02);
  emptiness = grtDistance(TRIGPIN_01,ECHOPIN_01);
  Serial.println("emptiness");
  Serial.println(emptiness);
  binStatus(emptiness);
  if(distance<60){
    runThreeQuarter();
    delay(10000);
    digitalWrite(DIRPIN,LOW);
    runThreeQuarter();
    digitalWrite(DIRPIN,HIGH);
  }
}
