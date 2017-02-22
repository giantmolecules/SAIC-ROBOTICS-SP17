/*
Brett Ian Balogh
ARTTECH 4015 | Robotics
22.February, 2017
Connect sonar sensor to pin A1
Prints the analog values from the sonar sensor to the terminal
*/

int analogVal = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  analogVal = analogRead(1);
  Serial.println(analogVal);
}
