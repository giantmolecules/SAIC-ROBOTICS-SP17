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
    accel.begin();
    mag.begin();
    dof.begin();
    
    // Start neopixel strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    
    // Start the timer that controls the light bar
    lightTimer.start();
    
    // Experimenting with alexa
    Particle.function("onoff", onoffLight);
    
}

//
// MAIN LOOP
//
  
void loop(){
    
//
// Do sensor Stuff
//

// Read Light Sensor
lightVal = analogRead(PHOTO_PIN);
    Serial.print("Light: ");
    Serial.print(lightVal);
    Serial.print("; ");

// Read Sonar Sensor
sonarVal = analogRead(SONAR_PIN);
    Serial.print("Sonar: ");
    Serial.print(sonarVal);
    Serial.print("; ");

// Read Accelerometer and Magnetometer
accel.getEvent(&accel_event);
mag.getEvent(&mag_event);

if (dof.accelGetOrientation(&accel_event, &orientation))
{
  Serial.print(("Roll: "));
  Serial.print(orientation.roll);
  Serial.print(("; "));
  Serial.print(("Pitch: "));
  Serial.print(orientation.pitch);
  Serial.print(("; "));
}

if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
{
  // 'orientation' should have valid .heading data now
}


if (dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accel_event))
{
  // Do something with the compensated data in mag_event!
  Serial.print(F("Heading: "));
  Serial.print(orientation.heading);
  Serial.println(("; "));
}

//
// Do Light and Sound Stuff
//
if(lightVal < 2000 && floodOn == false){
    floodLight();
    floodOn = true;
    
}
else{
    floodOn = false;
    //updateLightBar(10);
}



if(sonarVal < 200){
    int sonarValConstrained = constrain(sonarVal, 100,200);
    int sonarValMapped = map(sonarVal, 100,200, 1000,500);
    chirp(sonarValMapped);
}
else{
    noTone(BUZZ_PIN);// Do Nothing
}

//
// Do Motion Stuff
//

    // Main Loop Scheduler
    while(!moveTimer.isActive()&&sonarVal>300){
        
        switch(moves[i]){
            case 1:
            fwd(speeds[i], times[i]);
            break;
            
            case 2:
            rev(speeds[i], times[i]);
            break;
            
            case 3:
            left(speeds[i], times[i]);
            break;
            
            case 4:
            right(speeds[i], times[i]);
            break;
        
        }
        
        // increment index counter
        i++;
        
        // if increment index is out of bounds, set it to 0
        if(i >= arraySize(moves)){
            i=0;
        }
        
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
    //delay(fadeRate);
    lightBarCounter++;
    
}

void floodLight(){
        for(int i = 0; i < PIXEL_COUNT; i++){
            strip.setPixelColor(i, 255, 255, 255);
        }
    strip.show();
}

void chirp(int dist){
        tone(BUZZ_PIN, dist);
        delay(1);
}

int onoffLight(String args){
   int command = args.toInt();
   if(command == 1){
       setAllPixels(255);
       strip.setBrightness(255);
       strip.show();
       digitalWrite(STATUS_PIN, HIGH);
       return 1;
   }
   else{
       setAllPixels(0);
       strip.setBrightness(0);
       strip.show();
       digitalWrite(STATUS_PIN, LOW);
       return 0;
   }
}
   
   void setAllPixels(int c){
    for(int i = 0; i < PIXEL_COUNT; i++){
           strip.setPixelColor(i,c,c,c);
       }
       strip.show();
       delay(3000);
}

