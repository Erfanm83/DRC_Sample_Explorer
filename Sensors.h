class Sensors {
    public:

    int threshold_low;
    int threshold_high;
    int sensors_count;
    int *sensor_pins;
    int *errors;
    int previous_error;

    Sensors (int sensors_count, int *sensor_pins, int *errors, int threshold_low, int threshold_high) {
        this->threshold_low = threshold_low;
        this->threshold_high = threshold_high;
        this->sensor_pins = sensor_pins;
        this->sensors_count = sensors_count;
        this->errors = errors;
    }

    void initialize() {
      pinMode(sensor_pins[0], OUTPUT);
      pinMode(sensor_pins[1], INPUT);
      pinMode(sensor_pins[2], OUTPUT);
      pinMode(sensor_pins[3], INPUT);
    }
    
    int calculate_error() {
        int error;

        // Get distance from ultrasonic sensors'
        int front_distance = get_distance(sensor_pins[0] , sensor_pins[1]);
        int right_distance = get_distance(sensor_pins[2] , sensor_pins[3]);

        Serial.print("front_distance : ");
        Serial.println(front_distance);
        Serial.print("right_distance : ");
        Serial.println(right_distance);

        // Calculate error based on the difference in distances
        int distance_difference = front_distance - right_distance;
        //if positive : go straight
        //if negative : go right
        
        // Set Distance from right
        if (right_distance > threshold_low) {
            error = errors[1];
            // Turn right
            if (front_distance <= threshold_high + 5) {
                error = errors[4];
            }
        }
        // Go straight
        else if (right_distance <= threshold_low && front_distance >= threshold_high) {
            error = errors[2];
        }
        // Turn left
        else if (right_distance < threshold_low && front_distance < threshold_high) {
            error = errors[3];
        }
        // Continue straight
        else {
            error = errors[2];
        }

        // delay(50);
        return error;
    }

    int get_distance(int trig_sensor_pin , int echo_sensor_pin) {
      int duration, distance;
      //sending trigger pulse to signal
      digitalWrite(trig_sensor_pin , LOW);
      delayMicroseconds(2);
      digitalWrite(trig_sensor_pin , HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_sensor_pin , LOW);
      duration = pulseIn(echo_sensor_pin, HIGH);
      distance = (duration / 2) / 29.1;
      return distance;
    }
    
};
