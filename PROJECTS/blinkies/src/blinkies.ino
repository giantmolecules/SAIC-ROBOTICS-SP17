/*
Brett Ian Balogh
brettbalogh@gmail.com
Blinkies!
SAIC Robotics SP17
08.February, 2017
*/

int buttonVal = 0;

void setup() {
    pinMode(1, INPUT_PULLUP);
    pinMode(7, OUTPUT);
}

void loop() {
    buttonVal = digitalRead(1);
    //Particle.publish("buttonVal", String(buttonVal));
    digitalWrite(7, !buttonVal);
    disco();
}

void disco(){
    RGB.control(true);
    for(int i = 0; i < 256; i++){
        RGB.color(i, 0, 255-i);
        delay(50);
    }
    RGB.control(false);
}
