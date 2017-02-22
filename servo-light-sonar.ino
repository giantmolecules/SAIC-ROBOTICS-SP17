/*
ARTTECH 4015 | Robotics
Servo Scheduler
by Brett Ian Balogh
15. February, 2017
*/

// Create Servo objects
Servo servoLeft;
Servo servoRight;

// Define sensor pins

int lightPin = 0;
int sonarPin = 1;

// Create Variables to hold sensor values

int lightVal = 0;
int sonarVal = 0;

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

void setup() {
    
    // Attach servos to pins
    servoLeft.attach(0);
    servoRight.attach(1);
    
    // Enable serial port for debugging
    Serial.begin(9600);
}

void loop() {
    
    lightVal = analogRead(lightPin);
    
    if(lightVal < 3500){
        spotLight(true);
    }
    else{
        spotLight(false);
    }
    
    sonarVal = analogRead(sonarPin);
    Serial.println(sonarVal);
    
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

void spotLight(bool s){
    if(s==1){
        RGB.control(true);
        RGB.color(255,255,255);
        RGB.brightness(255);
    }
    if(s==0){
        RGB.control(false);
    }
}
