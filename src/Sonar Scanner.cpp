#include <Arduino.h>
#include <Servo.h>

Servo myservo;  

int trig = 10;
int echo = 11;
int buzzer = 9;

int red = 4;
int green = 5;
int blue = 6;

long duration;
float distance;
bool isDetected = false; 

void setup() {
  Serial.begin(9600);
  myservo.attach(3); 
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void setColor(bool redOn, int g, int b) {
  digitalWrite(red, redOn ? HIGH : LOW);
  analogWrite(green, g);
  analogWrite(blue, b);
}

void scanAndDetect() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.017;

  if (distance > 40) {
    // IDLE: PURPLE (Red HIGH + High Blue)
    setColor(true, 0, 220); 
    noTone(buzzer);
    isDetected = false;

  } else if (distance > 10) {
    // WARNING: ORANGE (Red HIGH + Low Green)
    setColor(true, 40, 0); 
    tone(buzzer, 1000);
    isDetected = false;

  } else {
    // ALARM: HOT PINK (Red HIGH + Low Blue, Green completely OFF)
    setColor(true, 0, 60); 
    tone(buzzer, 2000);
    
    if (!isDetected) {
      Serial.println("RETARD DETECTED!!!!");
      isDetected = true;
    }
  }
}

void loop() {
  for(int deg = 0; deg <= 180; deg++) {
    myservo.write(deg);             
    delay(20);
    scanAndDetect();
  }

  for (int deg = 180; deg >= 0; deg--) {
    myservo.write(deg);
    delay(20);
    scanAndDetect();
  }
}