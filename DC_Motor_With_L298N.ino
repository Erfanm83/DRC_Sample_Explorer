#include <Servo.h>
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"

// create servo objects to control servo motors
Servo rgbSensorServo;
Servo liverageServo;
Servo gripperServo;
// ==================================== SETTINGS ==================================/////////////////
// const int THRESHOLD = 5;
const int THRESHOLD_LOW = 5;
const int THRESHOLD_HIGH = 10;
const int SENSORS_COUNT = 4;//not needed
const int SENSOR_PINS[SENSORS_COUNT] = {12, 11, 4, 3};
const int ERRORS[3] = {0, 1, 2};

const int MAX_SPEED = 180;
const int NORMAL_SPEED = 150;
const int MIN_SPEED = 0;

const int LEFT_IN1 = 9;
const int LEFT_IN2 = 8;
const int LEFT_EN = 10;

const int RIGHT_IN1 = 7;
const int RIGHT_IN2 = 6;
const int RIGHT_EN = 5;

//UltraSonics' Pins
const int FRONT_TRIG = 12;
const int FRONT_ECHO = 11;
const int RIGHT_TRIG = 4;
const int RIGHT_ECHO = 3;

const float KP = 5;
const float KI = 3;
const float KD = 0;

const bool DEBUG = false;
const int DELAY_TIME = 50;

// ==============================================  /////////////////
//variable to store the servo motors position
int rgbSensorpos = 0;
int liveragepos = 0;
int gripperpos = 0;

Sensors sensors(SENSORS_COUNT, SENSOR_PINS, ERRORS, THRESHOLD_LOW, THRESHOLD_HIGH);

Motors motors(
    NORMAL_SPEED, MIN_SPEED, MAX_SPEED,
    LEFT_IN1, LEFT_IN2, LEFT_EN,
    RIGHT_IN1, RIGHT_IN2, RIGHT_EN
);
PID pid(KP, KI, KD);

void setup()
{
  Serial.begin(9600);
  motors.initialize();
  sensors.initialize();

  //set servo pins on arduino board
  // rgbSensorServo.attach(0);
  // liverageServo.attach(1);
  // gripperServo.attach(2);
}
void loop() {
  int error = sensors.calculate_error();
  int speed_difference = pid.calculate_speed_difference(error);
  motors.drive(speed_difference, DEBUG);

  if(DEBUG) {
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("speed_difference: ");
      Serial.println(speed_difference);
      Serial.println("--------------------------------------");
      delay(DELAY_TIME);
  }
  // demoOne();
  // delay(1000);
  // demoTwo();
  // delay(1000);
  // int duration , distance;
  //sending trigger pulse to signal
  // digitalWrite(front_trigPin , LOW);
  // delayMicroseconds(2);
  // digitalWrite(front_trigPin , HIGH);
  // delayMicroseconds(10);
  // digitalWrite(front_trigPin , LOW);
  // triggerSignal(right_trigPin);
  // duration = pulseIn(front_echoPin, HIGH);
  // int front_distance = (duration / 2) / 29.1;
  // int right_distance = getDistance(right_echoPin);
  //Decision
  // if (front_distance >= 15){
  //   goForward();
  // }
  // else{
  //   stop();
  // }
  // else if (right_distance > 6 && right_distance < 10){
  //   Serial.println("set distance from right");
  //   goRight();
  //   delay(20);
  //   goLeft();
  //   Serial.print("distance from right : ");
  //   Serial.print(getDistance(right_echoPin));
  //   Serial.println(" cm");
  // }
  // else if (right_distance > 11){
  //   Serial.println("going to right...");
  //   for (int i = 0 ; i < 4; i++){
  //     goRight();
  //     delay(20);
  //   }
  // }
  // else if (front_distance < 14){
  //   Serial.println("going for stop...");
  //   stop();
  //   delay(20);
  // }
  //helps to control the rate at which distance measurements are taken
  delay(DELAY_TIME);
}
// void demoOne()
// {
//   // this function will run the motors in both directions at a fixed speed
//   // turn on motor A
//   digitalWrite(in1, HIGH);
//   digitalWrite(in2, LOW);
//   // set speed to 200 out of possible range 0~255
//   analogWrite(enA, 200);
//   // turn on motor B
//   digitalWrite(in3, HIGH);
//   digitalWrite(in4, LOW);
//   // set speed to 200 out of possible range 0~255
//   analogWrite(enB, 200);
//   delay(2000);
//   // now change motor directions
//   digitalWrite(in1, LOW);
//   digitalWrite(in2, HIGH);  
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, HIGH); 
//   delay(2000);
//   // now turn off motors
//   digitalWrite(in1, LOW);
//   digitalWrite(in2, LOW);  
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, LOW);
// }
// void demoTwo()
// {
//   // this function will run the motors across the range of possible speeds
//   // note that maximum speed is determined by the motor itself and the operating voltage
//   // the PWM values sent by analogWrite() are fractions of the maximum speed possible 
//   // by your hardware
//   // turn on motors
//   digitalWrite(in1, LOW);
//   digitalWrite(in2, HIGH);  
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, HIGH); 
//   // accelerate from zero to maximum speed

//   for (int i = 0; i < 256; i++) { 
//     analogWrite(enA, i); 
//     analogWrite(enB, i); 
//     delay(20); 
//   } 
//   // decelerate from maximum speed to zero 
//   for (int i = 255; i > 0; --i) {
//     analogWrite(enA, i);
//     analogWrite(enB, i);
//     delay(20);
//   } 
 
//   // now turn off motors
//   digitalWrite(in1, LOW);
//   digitalWrite(in2, LOW);  
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, LOW);  
// }

//turn to right direction
// void goRight(){
//   digitalWrite(in1, LOW);
//   digitalWrite(in2, HIGH);  
//   digitalWrite(in3, LOW);
//   digitalWrite(in4, HIGH); 
//   for (int i = 0; i < 50 ; i++){
//     analogWrite(enB, i);
//     delay(20);
//   }
// }
//turn to left direction
// void goLeft(){
//   digitalWrite(in1, HIGH);
//   digitalWrite(in2, LOW);  
//   digitalWrite(in3, HIGH);
//   digitalWrite(in4, LOW); 
//   for (int i = 0; i < 50 ; i++){
//     analogWrite(enA, i);
//     delay(20);
//   }
// }
//Break

// void gripIn(){
//   for (pos = 0; pos <= 20; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
//   for (pos = 20; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15ms for the servo to reach the position
//   }
// }
// void gripOut(){

// }
// void pullUp(){

// }
// void pullDown(){

// }
// void rotateUp(){

// }
// void rotateDown(){

// }