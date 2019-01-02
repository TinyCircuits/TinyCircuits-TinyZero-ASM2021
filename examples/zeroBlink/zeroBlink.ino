//-------------------------------------------------------------------------------
//  TinyCircuits TinyZero Blink
//  Last Updated 28 Nov 2018
//  
//  Blink! If an accelerometer is present, change the rate of blinking according
//  to the Z axis sensor value.
//
//  Written by Ben Rose for TinyCircuits, https://tinycircuits.com
//
//-------------------------------------------------------------------------------


#include <Wire.h>
#include "BMA250.h"

BMA250 accel;

void setup() {
  pinMode(13, OUTPUT);
  Wire.begin();
  delay(20);
  if (!accel.begin(BMA250_range_2g, BMA250_update_time_32ms)) {
    while (1) {
      digitalWrite(13, HIGH);
      accel.read();
      delay(pow(abs(accel.Z),0.75)*4);
      digitalWrite(13, LOW);
      accel.read();
      delay(pow(abs(accel.Z),0.75)*4);
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
