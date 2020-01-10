#include "BMA250.h"
#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

BMA250::BMA250()
{
}

int BMA250::begin(uint8_t range, uint8_t bw)
{
  //Detect address
  I2Caddress = BMA250_I2CADDR;
  Wire.beginTransmission(I2Caddress);
  if (Wire.endTransmission()) {
    I2Caddress++;
    Wire.beginTransmission(I2Caddress);
    if (Wire.endTransmission()) {
      I2Caddress = 0;
      return -1;
    }
  }
  
  setScale(range);
  setInterval(bw);
  
  return 0;
}

void BMA250::setScale(uint8_t range)
{
  //Setup the range measurement setting
  Wire.beginTransmission(I2Caddress);
  Wire.write(0x0F);
  Wire.write(range);
  Wire.endTransmission();
  switch(range){
    case BMA250_range_2g:
      maxMpSS = 2.0*9.81;
      break;
    case BMA250_range_4g:
      maxMpSS = 4.0*9.81;
      break;
    case BMA250_range_8g:
      maxMpSS = 8.0*9.81;
      break;
    case BMA250_range_16g:
      maxMpSS = 16.0*9.81;
      break;
  }
}

void BMA250::setInterval(uint8_t bw)
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(0x10);
  Wire.write(bw);
  Wire.endTransmission();
  switch(bw){
    case BMA250_update_time_64ms:
      interval = 64;
      break;
    case BMA250_update_time_32ms:
      interval = 32;
      break;
    case BMA250_update_time_16ms:
      interval = 16;
      break;
    case BMA250_update_time_8ms:
      interval = 8;
      break;
    case BMA250_update_time_4ms:
      interval = 4;
      break;
    case BMA250_update_time_2ms:
      interval = 2;
      break;
    case BMA250_update_time_1ms:
      interval = 1;
      break;
    case BMA250_update_time_05ms:
      interval = 0;
      break;
  }
}

void BMA250::read()
{
  //Set register index
  Wire.beginTransmission(I2Caddress);
  Wire.write(0x02);
  Wire.endTransmission();
  //Request seven data bytes
  Wire.requestFrom(I2Caddress, 7);
  //Receive acceleration measurements as 16 bit integers
  X = (int16_t)Wire.read();
  X |= (int16_t)Wire.read() << 8;
  Y = (int16_t)Wire.read();
  Y |= (int16_t)Wire.read() << 8;
  Z = (int16_t)Wire.read();
  Z |= (int16_t)Wire.read() << 8;
  //Only use the 10 significant bits
  X >>= 6; Y >>= 6; Z >>= 6;
  Xmss = (float(X)/512.0)*maxMpSS;
  Ymss = (float(Y)/512.0)*maxMpSS;
  Zmss = (float(Z)/512.0)*maxMpSS;
  //Receive temperature measurement
  rawTemp = Wire.read();
  tempC = float(rawTemp)*0.5 + 24.0;
  //set up measurement timing
  newRead = millis() + interval;
}

bool BMA250::newReadAvailable()
{
  return((newRead-millis()) > interval);
}