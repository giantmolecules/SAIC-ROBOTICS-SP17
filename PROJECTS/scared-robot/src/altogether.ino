// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_9DOF.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_L3GD20_U.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_LSM303_U.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_Sensor.h"

// Define Pins
// D1 is I2C SDA
// D2 is I2C SCL
#define LEFT_SERVO D2
#define RIGHT_SERVO D3
#define BUZZ_PIN A5
// D5 is currently unused
#define PIXEL_PIN D6
#define STATUS_PIN D7
#define SONAR_PIN A0
#define PHOTO_PIN A1

// Sonar sensor thresholds

#define CLOSE_THRESHOLD 200
#define FAR_THRESHOLD 300

// IMPORTANT: Set pixel COUNT, TYPE

#define PIXEL_COUNT 10
#define PIXEL_TYPE WS2812B

// Create Neopixel Strip

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Create variables for sensor information

sensors_event_t accel_event;
sensors_event_t mag_event;
sensors_vec_t   orientation;

// Create sensors

Adafruit_LSM303_Mag_Unified   mag(30302);
Adafruit_LSM303_Accel_Unified accel;
Adafruit_9DOF dof;

// Create Servo objects
Servo servoLeft;
Servo servoRight;

// Create Variables to hold sensor values

int lightVal = 0;
int sonarVal = 0;

// Light Bar

int lightBarCounter = 0;
int lightBarPos = 0;
//int lightBar[] = {0, 63, 127, 255, 255, 127, 63, 0};
int lightBar[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool floodOn = false;

// Moves list: use the codes below to make a list of moves
// 1=fwd, 2=rev, 3=left, 4=right, 5=stop
int moves[] = {1,2,3,4};

// Speeds list: enter speeds for moves. 0-90

int speeds[] = {10, 20, 30, 40};

// Times list: enter movement times in milliseconds
int times[] = {2000, 2000, 2000, 2000};

// Counter variable for list index
int i = 0;

// Create Timer object
Timer moveTimer(1000, stop, true);
Timer lightTimer(50, updateLightBar);

//
// SETUP
//

void setup(){

    // Attach servos to pins
    servoLeft.attach(LEFT_SERVO);
    servoRight.attach(RIGHT_SERVO);

    // Start Serial Port
    Serial.begin(9600);

    // Start sensors
    // *NOTE: Photon will hang if sensor is not found.
    // Light will breathe green and not respond to flash.
    accel.begin();
    mag.begin();
    dof.begin();

    // Start neopixel strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    // Start the timer that controls the light bar
    lightTimer.start();

}

//
// MAIN LOOP
//

void loop(){

//
// Do sensor Stuff
//

// Read Sonar Sensor
sonarVal = analogRead(SONAR_PIN);
    Serial.print("Sonar: ");
    Serial.print(sonarVal);
    Serial.print("; ");

// Define a condition for action

if(sonarVal < FAR_THRESHOLD){

    // Constrain sensor value to be within CLOSE and FAR THRESHOLDS
    int sonarValConstrained = constrain(sonarVal, CLOSE_THRESHOLD,FAR_THRESHOLD);

    // Map the constrained value to an appropriate output range for brightness
    int sonarValMapped = map(sonarValConstrained, CLOSE_THRESHOLD,FAR_THRESHOLD, 255,0);

    // Set brightness based on proximity
    strip.setBrightness(sonarValMapped);

    // Move backwards a bit (speed=50, time=0.5sec)
    rev(50, 500);

}
else{
    // Do Nothing
}

}

//
// END MAIN LOOP
//

// Motion functions

void fwd(int speed, int runTime){
    Serial.println("FWD");
    moveTimer.changePeriod(runTime);
    moveTimer.start();
    servoLeft.write(90+speed);
    servoRight.write(90-speed);
}

void rev(int speed, int runTime){
    Serial.println("REV");
    moveTimer.changePeriod(runTime);
    moveTimer.start();
    servoLeft.write(90-speed);
    servoRight.write(90+speed);
}

void right(int speed, int runTime){
    Serial.println("RIGHT");
    moveTimer.changePeriod(runTime);
    moveTimer.start();
    servoLeft.write(90+speed);
    servoRight.write(90+speed);
}

void left(int speed, int runTime){
    Serial.println("LEFT");
    moveTimer.changePeriod(runTime);
    moveTimer.start();
    servoLeft.write(90-speed);
    servoRight.write(90-speed);
}

void stop(){
    Serial.println("STOP");
    servoLeft.write(90);
    servoRight.write(90);
}

// Light Functions

void updateLightBar(){

    int quotient = lightBarCounter/(PIXEL_COUNT-3);
    int remainder = lightBarCounter%(PIXEL_COUNT-3);
    int oddEven = quotient%2;

   if(oddEven == 1){
        // Odd
        lightBarPos--;
   }
   else{
       // Even
        lightBarPos++;
   }

    for(int i = 0; i < PIXEL_COUNT; i++){
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.setPixelColor(lightBarPos, 255,0,0);
    strip.show();
    lightBarCounter++;
}
