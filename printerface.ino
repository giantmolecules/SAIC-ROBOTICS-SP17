// ARTTECH 4015 SP19
// ASSIGNMENT 02
// PRINTERFACE TEMPLATE
// BRETT BALOGH
// bbalogh@saic.edu

// This example blinks LED1 if SW1 is pressed
// and blinks LED2 if SW2 is pressed.
// Modify the loop() function to create custom interactions.

// Include the debounce library to tame our switches

#include "Debounce.h"

// Define literals so we don't use variable space

#define LED1 0
#define LED2 1
#define SW1  2
#define SW2  3

// Create instances of the debouncer for each switch

Debounce debouncerSW1 = Debounce();
Debounce debouncerSW2 = Debounce();

// Create variables to hold the state of the switches

bool stateSW1 = false;
bool stateSW2 = false;

// Setup function for things that happen only once...

void setup(){

  // Set the pin modes for the LEDs to output

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  //pinMode(SW1, INPUT);
  //pinMode(SW2, INPUT);

  // Attach the debouncers to their switches

  debouncerSW1.attach(SW1, INPUT);
  debouncerSW2.attach(SW2, INPUT);

  // Set the debounce interval. Adjust as necessary.

  debouncerSW1.interval(20); // interval in ms
  debouncerSW2.interval(20); // interval in ms

}

// Loop function executes as long as Photon has power.

void loop(){

  // Update the debouncers

  debouncerSW1.update();
  debouncerSW2.update();

  // Read the switches

  stateSW1 = debouncerSW1.read();
  stateSW2 = debouncerSW2.read();

  // If SW1 is pressed, blink LED1

  if(stateSW1 == true){
    blink(LED1, 50, 100, 10);
  }

  // If SW2 is pressed, blink LED2

  if(stateSW2 == true){
    blink(LED2, 50, 100, 10);
  }

}

// Custom blink function
// first argument is the pin numner of the LED
// second argument is the ON time in milliseconds (1sec=1000ms)
// third argument is the OFF time in milliseconds
// forth argument is the number of ON/OFF cycles
// call it like this: blink(LED1, 500, 500, 10);
// this blinks LED1 (pin 0) ON 0.5s, OFF 0.5s, 10 times

void blink(int led, int dur1, int dur2, int num){
  for(i = 0; i < num; i++){
    digitalWrite(led, HIGH);
    delay(dur1);
    digitalWrite(led, LOW);
    delay(dur2);
  }
}
