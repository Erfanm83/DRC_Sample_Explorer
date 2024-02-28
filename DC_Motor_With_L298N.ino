// NEW CODE
/////////////////==================================== LIBRARIES ====================================/////////////////
#include <Servo.h>
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"
#include <Wire.h>
#include <Adafruit_TCS34725.h>
/////////////////================================ PRIMITIVE VALUES ================================/////////////////
int THRESHOLD_LOW = 6;
int THRESHOLD_HIGH = 11;
int SENSORS_COUNT = 4;//not needed
int SENSOR_PINS[4] = {12, 11, 4, 3};
int ERRORS[5] = {0, 1, 2, 3, 4};

int MAX_SPEED = 120;
int NORMAL_SPEED = 77;
int MIN_SPEED = 50;//it was 40

int LEFT_IN1 = 7;
int LEFT_IN2 = 6;
int LEFT_EN = 5;

int RIGHT_IN1 = 9;
int RIGHT_IN2 = 8;
int RIGHT_EN = 10;

//UltraSonics' Pins
int FRONT_TRIG = 12;
int FRONT_ECHO = 11;
int RIGHT_TRIG = 4;
int RIGHT_ECHO = 3;

float KP = 10;
float KI = 5;
float KD = 0;

const bool DEBUG = true;
const int DELAY_TIME = 500;

bool firstAidZone = false;
bool inTargetZone = false;
bool isred = false;
bool isgreen = false;
bool isblue = false;

// Create an instance of the TCS34725 sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Sensors sensors(SENSORS_COUNT, SENSOR_PINS, ERRORS, THRESHOLD_LOW, THRESHOLD_HIGH);

Motors motors(
    NORMAL_SPEED, MIN_SPEED, MAX_SPEED,
    LEFT_IN1, LEFT_IN2, LEFT_EN,
    RIGHT_IN1, RIGHT_IN2, RIGHT_EN
);
PID pid(KP, KI, KD);

// create servo objects to control servo motors
Servo rgbSensorServo;
Servo liverageServo;
Servo gripperServo;

void setup() {
    Serial.begin(9600);
    motors.initialize();
    sensors.initialize();

    // Initialize the RGB Light sensor
    if (!tcs.begin()) {
        Serial.println("Could not find a valid TCS34725 sensor, check wiring!");
    }
}

void loop() {
    int error;
    int speed_difference = 0;
    
    //on Rescue Zone
    if(firstAidZone == false) {

        error = sensors.calculate_error();
        //keep liverage servo up in order not to have a collision with walls
        if(error == 2) {
            motors.goStraight(DEBUG);
        }
        else if(error == 4) {
            motors.turnRight(DEBUG);
        }
        else if (error == 1) {
            speed_difference = pid.calculate_speed_difference(error);
            motors.drive(speed_difference, DEBUG);
        }
        else if(error == 3) {
            motors.turnLeft(DEBUG);
        }
    }

    //on FirstAid Zone
    if(sensors.detectColor(true) == "black" && !firstAidZone) {
        sensors.disableSensor();
        rgbSensorServo.attach(A0);
        rgbSensorServo.write(0);
        delay(50);
        //pull up rgb sensor
        pull(rgbSensorServo, 0, 87, true);
        firstAidZone = true;
        rgbSensorServo.detach();
        sensors.enableSensor();
        delay(500);
    }
    
    // looking for boxes at front
    if(firstAidZone == true) {
        Serial.println("on First Aid Zone...");
        while(isblue == false && isgreen == false) {
            Serial.println("on while loop");
            if(sensors.detectColor(false) != "ERROR") {
                Serial.println("On while loop");
                motors.drive(-10 , true);
                //picking Up blue Box
                if(sensors.detectColor(false) == "blue") {
                    sensors.disableSensor();
                    motors.rollBack(DEBUG);
                    delay(500);
                    pickUp(liverageServo ,gripperServo ,rgbSensorServo);
                    Serial.println();
                    Serial.println("Now, Picking Up blue box");
                    isblue = true;
                    inTargetZone = true;
                    delay(500);
                }
                //picking Up green box
                else if (sensors.detectColor(false) == "green") {
                    // pickUp(liverageServo ,gripperServo ,rgbSensorServo);
                    // Serial.println("Now, Picking Up blue box");
                    isgreen = true;
                    inTargetZone = true;
                    delay(500);
                }
            }
            else
                Serial.println("ERROR Getting Data From tcs sensor");
            
        }
    }

    //looking for circle area on the ground
    if (inTargetZone == true) {
        Serial.println("on Target Zone...");
        //putting Down red Box
        if(sensors.detectColor(false) == "blue" && isblue == true) {
            // putDown(liverageServo ,gripperServo ,rgbSensorServo);
            // motors.turnLeft(DEBUG);
            // Serial.println("blue Box Successfully Put Down !");
            delay(500);
        }
        //putting Down green Box
        else if(sensors.detectColor(false) = "green" && isgreen == true){
            // putDown(liverageServo ,gripperServo ,rgbSensorServo);
            // Serial.println("Task Successfully Completed !");
            // delay(500);
        }
    }
    
    //DEBUG Mode
    if(DEBUG) {
        Serial.print("error: ");
        Serial.println(error);
        Serial.print("speed_difference: ");
        Serial.println(speed_difference);
        Serial.println("--------------------------------------");
        //Adjust delay helps to control the rate at which distance measurements are taken
        delay(DELAY_TIME);
    }
}

  //pick Up boxes from the area
  void pickUp(Servo liverageServo , Servo gripperServo, Servo rgbSensorServo) {
      liverageServo.attach(A1);
      gripperServo.attach(A2);
      // liverageServo.write(15);
      // gripperServo.write(55);
      //pull down gripper mechanism
      pull(liverageServo, 15 , 80, true);
      delay(3000);
      //close the gripper
      pull(gripperServo, 45, 85, true);
      delay(3000);
      //pull up gripper mechanism
      pull(liverageServo, 15 , 68, false);
      delay(100);
      //pull down rgb sensor
      pull(rgbSensorServo, 0, 87, false);
      liverageServo.detach();
      gripperServo.detach();
  }

  //put Down boxes from the area
  void putDown(Servo liverageServo , Servo gripperServo, Servo rgbSensorServo) {
      liverageServo.attach(A1);
      gripperServo.attach(A2);
      //pull down gripper mechanism
      pull(liverageServo, 15 , 68, true);
      delay(3000);
      //open the gripper
      pull(gripperServo, 45, 85, false);
      delay(3000);
      //pull up gripper mechanism
      pull(liverageServo, 15 , 68, false);
      liverageServo.detach();
      gripperServo.detach();  
  }

  //rgb :
  //startPoint : 0
  //endPoint : 87

  //liverage:
  //Upmode = false bayad bashe ta bala beare
  //pullDown startPoint : 15
  //pullDown endPoint : 68

  //gripper:
  //Upmode = true bashe mibande
  //       = false bashe bazmikone
  //startPoint : 45
  //endPoint : 85

  //decision for state of servo motors of the robot
  //based on startPoint & endPoint & Upmode of servo motors
  void pull(Servo servo, int startPoint, int endPoint, bool Upmode) {
      int pos = 0;
      //Up Mode
      if (Upmode == true) {
          for (pos = startPoint; pos < endPoint; pos += 1) {
              // in steps of 1 degree
              servo.write(pos);              // tell servo to go to position in variable 'pos'
              // waits 30ms for the servo to reach the position
              delay(60);
          }
      }
      //Down Mode
      else{
          for (pos = endPoint; pos > startPoint; pos -= 1) {
              servo.write(pos);              // tell servo to go to position in variable 'pos'
              // waits 30ms for the servo to reach the position
              delay(30);
          }
      }
      delay(50);
  }
