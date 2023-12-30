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

    Motors (int normal_speed, int min_speed, int max_speed, int left_in1, int left_in2, int left_en, int right_in1, int right_in2, int right_en) {
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
    }

    void set_motor(int in1, int in2, int en, int speed) {
        int *direction = get_direction(speed);
        
        digitalWrite(in1, direction[1]);
        digitalWrite(in2, direction[0]);
        // digitalWrite(in1, HIGH);
        // digitalWrite(in2, LOW);
        analogWrite(en, normalize_speed(speed));
    }

    int *get_direction(int speed) {
        int direction[2];
        if(speed > 0) {
            direction[0] = LOW;
            direction[1] = HIGH;
        } else {
            direction[0] = HIGH;
            direction[1] = LOW;
        }
        return direction;
    }

    int normalize_speed(int speed) {
        return constrain(abs(speed), min_speed, max_speed);
    }

    void drive(int speed_difference, bool debug) {
        int left_speed = normal_speed + speed_difference;
        int right_speed = normal_speed - speed_difference;

        set_motor(left_in1, left_in2, left_en, left_speed);
        set_motor(right_in1, right_in2, right_en, right_speed);

        if(debug) {
            Serial.print("left_speed: ");
            Serial.print(normalize_speed(left_speed));
            Serial.print(" - ");
            Serial.println(left_speed > 0 ? "F" : "B");

            Serial.print("right_speed: ");
            Serial.print(normalize_speed(right_speed));
            Serial.print(" - ");
            Serial.println(right_speed > 0 ? "F" : "B");
        }
    }

    void stop(){
    // decelerate from maximum speed to zero 
      for (int i = max_speed; i > min_speed; i-=1){
        analogWrite(left_en, i);
        analogWrite(right_en, i);
        delay(20);
      } 
      // now turn off motors
      digitalWrite(left_in1, LOW);
      digitalWrite(left_in2, LOW);  
      digitalWrite(right_in1, LOW);
      digitalWrite(right_in2, LOW);  
    }

    // void goForward(){
    // //turn on motors
    //     digitalWrite(left_in2, LOW);
    //     digitalWrite(left_in1, HIGH);
    //     digitalWrite(right_in2, LOW);
    //     digitalWrite(right_in1, HIGH);
    //     // accelerate from zero to maximum speed
    //     for (int i = min_speed; i < max_speed; i++) {
    //       analogWrite(left_en, i); 
    //       analogWrite(right_en, i); 
    //       delay(20);
    //     }
    // analogWrite(left_en, 100); 
    // analogWrite(right_en, 100);
    // delay(500);
    // analogWrite(left_en, 0); 
    // analogWrite(right_en, 0);
    // delay(500);
    // analogWrite(left_en, 100); 
    // analogWrite(right_en, 100);
    // delay(500);
    // }

};
