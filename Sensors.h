class Sensors {
    public:

    int threshold;
    //Coming Soon...
    int threshold_low;
    int threshold_high;
    int sensors_count;
    int *sensor_pins;
    int *errors;
    int previous_error;

    Sensors (int sensors_count, int *sensor_pins, int *errors, int threshold_low, int threshold_high) {
        // this->threshold = threshold;
        //Coming soon...
        this -> threshold_low;
        this -> threshold_high;
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
        int front_distance = get_distance(sensor_pin[1]);
        int right_distance = get_distance(sensor_pins[3]);

        // Calculate error based on the difference in distances
        int distance_difference = front_distance - right_distance;
        //if positive : go straight
        //if negative : go right
        //if zero or negative : go left

        //if both right_distance & front_distance were lower than th go left

         // Adjust the following values based on the robot's behavior
        // int threshold_low = -10;
        // int threshold_high = 10;

        // Determine the error based on the distance difference
        if (distance_difference < threshold_low) {
            error = errors[0];  // Turn right
        } else if (distance_difference > threshold_high) {
            error = errors[1];  // Go straight
        } else if (right_distance < threshold_high && front_distance < threshold_high){
            error = errors[2];  // Turn left
        } else {
            error = previous_error;  // Continue straight
        }
        
        previous_error = error;
        return error;
    }

    // // check if an obstacle is detected in front
    // bool obstacle_on_front(int threshold , int sensor_pin) {
    //     int front_distance = get_distance(sensor_pin);
    //     return front_distance < threshold;
    // }

    // // New method to check if an obstacle is detected on the right
    // bool obstacle_on_right(int threshold , int sensor_pin) {
    //     int right_distance = get_distance(sensor_pin);
    //     return right_distance < threshold;
    // }

    // bool on_right(int sensor_pin) {
    //   int sensor_value = analogRead(sensor_pin);
    //   return sensor_value < threshold;
    // }

    int get_distance(int sensor_pin) {
      int duration, distance;
      //sending trigger pulse to signal
      digitalWrite(sensor_pin , LOW);
      delayMicroseconds(2);
      digitalWrite(sensor_pin , HIGH);
      delayMicroseconds(10);
      digitalWrite(sensor_pin , LOW);
      duration = pulseIn(sensor_pin, HIGH);
      distance = (duration / 2) / 29.1;
      return distance;
    }
    
};
