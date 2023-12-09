//UltraSonics' Pins
int front_trigPin = 12;
int front_echoPin = 11;
int right_trigPin = 4;
int right_echoPin = 3;

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;
void setup()
{
  Serial.begin(9600);
  //Ultrasonic Sensor details set
  pinMode(front_trigPin , OUTPUT);
  pinMode(front_echoPin , INPUT);
  pinMode(right_trigPin , OUTPUT);
  pinMode(right_echoPin , INPUT);
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
void demoOne()
{
  // this function will run the motors in both directions at a fixed speed
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 200);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay(2000);
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  delay(2000);
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void demoTwo()
{
  // this function will run the motors across the range of possible speeds
  // note that maximum speed is determined by the motor itself and the operating voltage
  // the PWM values sent by analogWrite() are fractions of the maximum speed possible 
  // by your hardware
  // turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  // accelerate from zero to maximum speed

 for (int i = 0; i < 256; i++) { 
  analogWrite(enA, i); 
  analogWrite(enB, i); 
  delay(20); 
  } 
// decelerate from maximum speed to zero 
for (int i = 255; i > 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 

 
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}
void goForward(){
  //turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // accelerate from zero to maximum speed
 for (int i = 0; i < 256; i++) { 
    analogWrite(enA, i); 
    analogWrite(enB, i); 
    delay(20); 
  } 

}
//turn to right direction
void goRight(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  for (int i = 0; i < 50 ; i++){
    analogWrite(enB, i);
    delay(20);
  }
}
//turn to left direction
void goLeft(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  for (int i = 0; i < 50 ; i++){
    analogWrite(enA, i);
    delay(20);
  }
}
//Break
void stop(){
  // decelerate from maximum speed to zero 
  for (int i = 255; i > 0; --i){
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}

void loop()
{
  // demoOne();
  // delay(1000);
  // demoTwo();
  // delay(1000);
  long duration , distance;
  //sending trigger pulse to signal
  triggerSignal(front_trigPin);
  triggerSignal(right_trigPin);

  int front_distance = getDistance(front_echoPin);
  int right_distance = getDistance(right_echoPin);
  //Decision
  if (front_distance >= 15 && right_distance <= 5){
    Serial.println("going forward...");
    goForward();
  }
  else if (right_distance > 6 && right_distance < 10){
    Serial.println("set distance from right");
    goRight();
    delay(20);
    goLeft();
    Serial.print("distance from right : ");
    Serial.print(getDistance(right_echoPin));
    Serial.println(" cm");
  }
  else if (right_distance > 11){
    Serial.println("going to right...");
    for (int i = 0 ; i < 4; i++){
      goRight();
      delay(20);
    }
  }
  else if (front_distance < 14){
    Serial.println("going for stop...");
    stop();
    delay(20);
  }
  //helps to control the rate at which distance measurements are taken
  delay(500);
}
float getDistance(int given_echoPin){
  float duration = pulseIn(given_echoPin, HIGH);
  float distance = (duration / 2) / 29.1;
  return distance;
}
void triggerSignal(int given_trigPin){
  digitalWrite(given_trigPin , LOW);
  delayMicroseconds(2);
  digitalWrite(given_trigPin , HIGH);
  delayMicroseconds(10);
  digitalWrite(given_trigPin , LOW);
}