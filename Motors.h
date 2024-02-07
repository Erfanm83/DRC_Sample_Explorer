class Motors {
public:

  int normal_speed;
  int min_speed;
  int max_speed;

  int left_in1;
  int left_in2;
  int left_en;

  int right_in1;
  int right_in2;
  int right_en;

  bool complete;

  Motors(int normal_speed, int min_speed, int max_speed, int left_in1, int left_in2, int left_en, int right_in1, int right_in2, int right_en) {
    this->normal_speed = normal_speed;
    this->min_speed = min_speed;
    this->max_speed = max_speed;

    this->left_in1 = left_in1;
    this->left_in2 = left_in2;
    this->left_en = left_en;

    this->right_in1 = right_in1;
    this->right_in2 = right_in2;
    this->right_en = right_en;
  }

  void initialize() {
    pinMode(left_en, OUTPUT);
    pinMode(left_in1, OUTPUT);
    pinMode(left_in2, OUTPUT);

    pinMode(right_en, OUTPUT);
    pinMode(right_in1, OUTPUT);
    pinMode(right_in2, OUTPUT);
    complete = true;
  }

  void set_motor(int in1, int in2, int en, int speed) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(en, normalize_speed(speed));
  }

  int normalize_speed(int speed) {

    return constrain(abs(speed), min_speed, max_speed);

    // if(speed <= 50){
    //   return 40;
    // }else{
    //   return constrain(abs(speed), min_speed, max_speed);
    // }
  }

  void drive(int speed_difference, bool debug) {
    int left_speed = normal_speed + speed_difference;
    int right_speed = normal_speed - speed_difference;

    set_motor(left_in1, left_in2, left_en, left_speed);
    set_motor(right_in1, right_in2, right_en, right_speed);

    if (debug) {
      Serial.print("NOW , set distance from right...  left_speed: ");
      Serial.print(normalize_speed(left_speed));
      Serial.print(" - ");
      Serial.println(left_speed > 0 ? "F" : "B");

      Serial.print("NOW , set distance from right...  right_speed: ");
      Serial.print(normalize_speed(right_speed));
      Serial.print(" - ");
      Serial.println(right_speed > 0 ? "F" : "B");
    }
  }

  void turnLeft(bool debug){
      
      int left_speed = normal_speed - 30;
      int right_speed = normal_speed + 90;

      // for(int i = 0 ; i < 3; i++){

      digitalWrite(right_in1, HIGH);
      digitalWrite(right_in2, LOW);
      analogWrite(right_en, normalize_speed(right_speed));

      digitalWrite(left_in1, LOW);
      digitalWrite(left_in2, HIGH);
      analogWrite(left_en, normalize_speed(left_speed));

      delay(50);

      // set_motor(left_in1 , left_in2, left_en, left_speed);
      // set_motor(right_in1, right_in2, right_en, right_speed);
      // }

      if (debug) {
      Serial.print("NOW , turn Left...  left_speed: ");
      Serial.print(normalize_speed(left_speed));
      Serial.print(" - ");
      Serial.println(left_speed > 30 ? "T" : "F");

      Serial.print("NOW , turn Left...  right_speed: ");
      Serial.print(normalize_speed(right_speed));
      Serial.print(" - ");
      Serial.println(right_speed > 80 ? "T" : "F");
      }
    
  }

  void turnRight(bool debug) {
      
      int left_speed = normal_speed + 90;
      int right_speed = normal_speed - 30;

      set_motor(left_in1 , left_in2, left_en, 80);
      set_motor(right_in1, right_in2, right_en, 80);
      delay(50);
      set_motor(left_in1 , left_in2, left_en, left_speed);
      set_motor(right_in1, right_in2, right_en, right_speed);

      // digitalWrite(right_in1, LOW);
      // digitalWrite(right_in2, HIGH);
      // analogWrite(right_en, normalize_speed(right_speed));

      // digitalWrite(left_in1, HIGH);
      // digitalWrite(left_in2, LOW);
      // analogWrite(left_en, normalize_speed(left_speed));

      // delay(50);

      if (debug) {
      Serial.print("NOW , turn Right...  left_speed: ");
      Serial.print(normalize_speed(left_speed));
      Serial.print(" - ");
      Serial.println(left_speed > 80 ? "T" : "F");

      Serial.print("NOW , turn Right...  right_speed: ");
      Serial.print(normalize_speed(right_speed));
      Serial.print(" - ");
      Serial.println(right_speed > 30 ? "T" : "F");
      }
  }

  void goStraight(bool debug){

      int left_speed = normal_speed;
      int right_speed = normal_speed;

      set_motor(left_in1 , left_in2, left_en, left_speed);
      set_motor(right_in1, right_in2, right_en, right_speed);

      if (debug) {
      Serial.print("NOW , go Straight...  left_speed: ");
      Serial.print(normalize_speed(left_speed));
      Serial.print(" - ");
      Serial.println(left_speed > 80 ? "T" : "F");

      Serial.print("NOW , go Straight...  right_speed: ");
      Serial.print(normalize_speed(right_speed));
      Serial.print(" - ");
      Serial.println(right_speed > 80 ? "T" : "F");
      }
  }
};