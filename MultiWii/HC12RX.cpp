/*
MIT License

Copyright (c) 2021 Rupak Poddar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <NeoSWSerial.h>
#include "Arduino.h"
#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "HC12RX.h"

#if defined(HC12RX)

#define PADDING 10
#define THROTTLE_INC 11

int16_t HC12_rcData[RC_CHANS];

NeoSWSerial HC12(7, 8);

unsigned long lastReceiveTime = 0, lastRunTime = 0;
const char delimiter = ',';
byte boundLow;
byte boundHigh;
String input;

int throttle = 0;
int yaw = 512;
int pitch = 512;
int roll = 512;
int aux1 = 1;
int aux2 = 0;

void HC12_Init() {
  HC12.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void HC12_Read_RC() {	
  unsigned long currentTime = millis();
  if((currentTime - lastRunTime) >= 100){
    int numDelimiter = 0;
    if(HC12.available() > 0){
      input = HC12.readStringUntil('\n');
      for(int i = 0; i < input.length(); i++){
        if(input[i] == delimiter){
          numDelimiter++;
        }
      }
      if(numDelimiter == 5){
        //Serial.println(input); // <---
        lastReceiveTime = millis();
      
        boundLow = input.indexOf(delimiter);
        int throttle_det = input.substring(0, boundLow).toInt();
        if(throttle_det >= 600){
          if(throttle <= (1023 - THROTTLE_INC)){
            throttle += THROTTLE_INC;
          }
        }

        if(throttle_det <= 424){
          if(throttle >= THROTTLE_INC){
            throttle -= THROTTLE_INC;
          }
        }
        
        boundHigh = input.indexOf(delimiter, boundLow+1);
        yaw = input.substring(boundLow+1, boundHigh).toInt();
      
        boundLow = input.indexOf(delimiter, boundHigh+1);
        pitch = input.substring(boundHigh+1, boundLow).toInt();

        boundHigh = input.indexOf(delimiter, boundLow+1);
        roll = input.substring(boundLow+1, boundHigh).toInt();

        boundLow = input.indexOf(delimiter, boundHigh+1);
        aux1 = input.substring(boundHigh+1, boundLow).toInt();
        /*
        if(aux1 == 0){
          // Do nothing
        }
        else{
          // Do nothing
        }
        */
        aux2 = input.substring(boundLow+1).toInt();

        if((throttle_det < PADDING) && (yaw < PADDING) && (roll > (1023 - PADDING)) && (pitch < PADDING)){
          throttle = 0;
        }
      }   
    }
    currentTime = millis();
    if (currentTime - lastReceiveTime > 1000) {
      digitalWrite(LED_BUILTIN, HIGH);
      if(throttle >= (THROTTLE_INC/5)){
        throttle -= (THROTTLE_INC/5);
      }
      yaw = 512;
      pitch = 512;
      roll = 512;
      aux1 = 1;
      aux2 = 0;
      //Serial.println("Connection Lost"); // <---
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      HC12.println("OK");
    }
    lastRunTime = millis();
    HC12_rcData[THROTTLE] = map(throttle, 0, 1023, 1000, 2000);
    HC12_rcData[YAW] = map(yaw, 0, 1023, 1000, 2000);
    HC12_rcData[PITCH] = map(pitch, 0, 1023, 1000, 2000);
    HC12_rcData[ROLL] = map(roll, 0, 1023, 1000, 2000);
    HC12_rcData[AUX1] = map(aux1, 0, 1, 2000, 1000);
    HC12_rcData[AUX2] = 1000;
  }
}
#endif
