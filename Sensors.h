#include <Adafruit_TCS34725.h>

class Sensors {
    public:

    // Adjust these threshold values based on your environment and lighting conditions
    int redThreshold = 170;  // threshold for red color
    int greenThreshold = 80;  // threshold for green color
    int blueThreshold = 80;  // threshold for blue color
    int blackThreshold = 80; // threshold for black color

    int threshold_low;
    int threshold_high;
    int sensors_count;
    int *sensor_pins;
    int *errors;
    int previous_error;
    Adafruit_TCS34725 tcs;

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
        // Create an instance of the TCS34725 sensor
        tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
        tcs.begin();
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

    String detectColor(bool canBlack) {
        // Read RGB values from the sensor
        uint16_t r, g, b, c;
        tcs.enable();
        tcs.getRawData(&r, &g, &b, &c);

        // Print RGB values
        Serial.print("Red: "); Serial.print(r); Serial.print(" ");
        Serial.print("Green: "); Serial.print(g); Serial.print(" ");
        Serial.print("Blue: "); Serial.print(b); Serial.print(" ");

        //check to see not to have zero values 
        if(r != 0 && g != 0 && b != 0) {
          
          // Check if the detected color is black
          if(r < blackThreshold && g < blackThreshold && b < blackThreshold
           && g < greenThreshold && canBlack) {
              Serial.println("Detected Black Color! ");
              return "black";
          }
          else if (r < 66 && g < 98 && b < blueThreshold && canBlack == false) {
              Serial.println("Detected blue Color!");
              return "blue";
          }
          // Check if the detected color is green
          else if (r < 41 && g < greenThreshold && b < 60 && canBlack == false) {
              Serial.println("Detected Green Color!");
              return "green";
          }
          // Check if the detected color is red
          // else if (r < redThreshold && g > greenThreshold) {
          //   Serial.println("Detected Red Color!");
          //   return "red";
          // }
          // // Check if the detected color is blue

          return "white";
      }
      
      return "ERROR";
    }

    void disableSensor(){
      tcs.disable();
    }

    void enableSensor(){
      tcs.enable();
    }
    
};
