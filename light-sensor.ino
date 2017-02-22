/*
Brett Ian Balogh
ARTTECH 4015 | Robotics
22.February, 2017

Connect light sensor to pin A0

Prints the analog values from the light sensor to the terminal
*/

int analogVal = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  analogVal = analogRead(0);
  Serial.println(analogVal);
}
