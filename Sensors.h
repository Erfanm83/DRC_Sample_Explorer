class Sensors {
    public:

    int threshold;
    int sensors_count;
    int *sensor_pins;
    int *errors;
    int previous_error;

    Sensors (int sensors_count, int *sensor_pins, int *errors) {
        // this->threshold = threshold;
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
    // int calculate_error() {
    //     int error;
    //     switch (get_line_position()) {
    //         case 0b01111:
    //         case 0b00111:
    //         case 0b00011: error =  errors[3]; break;

    //         case 0b00001: error =  errors[2]; break;
            
    //         case 0b00110:
    //         case 0b00010: error =  errors[1]; break;

    //         case 0b11111:
    //         case 0b00100: error =  errors[0]; break;

    //         case 0b01100:
    //         case 0b01000: error = -errors[1]; break;

    //         case 0b10000: error = -errors[2]; break;
            
    //         case 0b11110:
    //         case 0b11100:
    //         case 0b11000: error = -errors[3]; break;
            
    //         default:      error =  previous_error; break;
    //     }
    //     previous_error = error;
    //     return error;
    // }
    int calculate_error() {
        int error;

        // Get distance from the ultrasonic sensor (replace 'UltrasonicSensorPin' with your actual pin)
        int distance = get_distance(sensor_pins[3]);

        // Adjust the error based on the distance from the wall
        if (distance > 0 && distance < 5) {
            // The robot is too close to the wall
            error = errors[3];
        } else if (distance >= 5 && distance < 7) {
            // The robot is somewhat close to the wall
            error = errors[2];
        } else if (distance >= 7 && distance < 10) {
            // The robot is at a moderate distance from the wall
            error = errors[1];
        } else if (distance >= 10) {
            // The robot is far from the wall
            error = errors[0];
        } else {
            // Default case, use the previous error
            error = previous_error;
        }

        // Update the previous error for the next iteration
        previous_error = error;

        return error;
    }

    // bool on_line(int sensor_pin) {
    //   int sensor_value = analogRead(sensor_pin);
    //   return sensor_value < threshold;
    // }

    bool on_right(int sensor_pin) {
      int sensor_value = analogRead(sensor_pin);
      return sensor_value < threshold;
    }

    // int get_line_position() {
    //     int line_position = 0;
    //       for(int i = 0; i < sensors_count; i++) {
    //         if(on_line(sensor_pins[i])) {
    //           line_position += (1 << i);
    //         }
    //       }
    //     return line_position;
    // }

    int get_distance(int sensor_pin) {
      //sending trigger pulse to signal
      digitalWrite(sensor_pin , LOW);
      delayMicroseconds(2);
      digitalWrite(sensor_pin , HIGH);
      delayMicroseconds(10);
      digitalWrite(sensor_pin , LOW);
      int duration = pulseIn(sensor_pin, HIGH);
      return (duration / 2) / 29.1;
      // if(on_right(sensor_pin)){
      //   distance += (1 << 3);
      // }
    }
    
};
