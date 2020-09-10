/*************************************************************************
 * BMA250 TinyZero Tutorial:
 * This example program will show the basic method of printing out the 
 * accelerometer values from the BMA250 to the Serial Monitor, and the
 * Serial Plotter
 * 
 * Hardware by: TinyCircuits
 * Code by: Laverena Wienclaw for TinyCircuits
 *
 * Initiated: Mon. 11/1/2018
 * Updated: Tue. 11/1/2018
 ************************************************************************/
 
#include <Wire.h>         // For I2C communication with sensor
#include "BMA250.h"       // For interfacing with the accel. sensor

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;
int x, y, z;
float xmss, ymss, zmss;
float temp;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.print("Initializing BMA...");
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_16g, BMA250_update_time_8ms); 
}

void loop() {
  if(accel_sensor.newReadAvailable()){
    accel_sensor.read();//This function gets new data from the acccelerometer

    // Get the acceleration values from the sensor and store them into global variables
    // (Makes reading the rest of the program easier)
    x = accel_sensor.X;
    xmss = accel_sensor.Xmss;
    y = accel_sensor.Y;
    ymss = accel_sensor.Ymss;
    z = accel_sensor.Z;
    zmss = accel_sensor.Zmss;
    temp = (accel_sensor.tempC);
  
    // If the BME280 is not found, nor connected correctly, these values will be produced
    // by the sensor 
    if (x == -1 && y == -1 && z == -1) {
      // Print error message to Serial Monitor
      Serial.print("ERROR! NO BMA250 DETECTED!");
    }
    
    else { // if we have correct sensor readings: 
      //showSerial();                 //Print to Serial Monitor 
      showGraph();           //Show on Serial Plotter
    }
  }
}

// Prints the sensor values to the Serial Monitor (found under 'Tools')
void showSerial() {
  SerialUSB.print("X = ");
  SerialUSB.print(xmss);
  
  SerialUSB.print("  Y = ");
  SerialUSB.print(ymss);
  
  SerialUSB.print("  Z = ");
  SerialUSB.print(zmss);
  
  SerialUSB.print("  Temperature(C) = ");
  SerialUSB.println(temp);
}

//Plots X, Y and Z data on Serial Plotter (found under 'Tools')
void showGraph() {
  SerialUSB.print(xmss);
  SerialUSB.print(",");

  SerialUSB.print(ymss);
  SerialUSB.print(",");

  SerialUSB.print(zmss);
  SerialUSB.println(",");
}
