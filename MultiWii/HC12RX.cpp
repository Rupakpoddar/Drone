#include <NeoSWSerial.h>
#include "Arduino.h"
#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "HC12RX.h"

#if defined(HC12RX)

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
        //Serial.println(input); //<---
        lastReceiveTime = millis();
      
        boundLow = input.indexOf(delimiter);
        int throttle_det = input.substring(0, boundLow).toInt();
        if(throttle_det >= 600){
          if(throttle <= 1013){
            throttle += 10;
          }
        }

        if(throttle_det <= 424){
          if(throttle >= 10){
            throttle -= 10;
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
    
        aux2 = input.substring(boundLow+1).toInt();
      }   
    }
    currentTime = millis();
    if (currentTime - lastReceiveTime > 1000) {
      digitalWrite(LED_BUILTIN, HIGH);
      if(throttle > 1){
        throttle -= 2;
      }
      yaw = 512;
      pitch = 512;
      roll = 512;
      aux1 = 1;
      aux2 = 0;
      //Serial.println("Connection Lost"); //<---
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
