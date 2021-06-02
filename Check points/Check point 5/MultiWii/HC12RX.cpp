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

//HC12Data HC12_Data; // <---
//HC12Payload HC12_Payload; // <---
//extern HC12Payload HC12_Payload; //<---
unsigned long lastReceiveTime = 0, lastRunTime = 0;
const char delimiter = ',';
byte boundLow;
byte boundHigh;
String input;

int throttle;
int yaw;
int pitch;
int roll;
int aux1;
int aux2;

/*
void resetHC12Data() 
{
  HC12_Data.throttle = 512;
  HC12_Data.yaw = 512;
  HC12_Data.pitch = 512;
  HC12_Data.roll = 512;
  HC12_Data.aux1 = 0;
  HC12_Data.aux2 = 0;
  HC12_Data.switches = 0;
}
*/

void resetHC12Data() 
{
  throttle = 512;
  yaw = 512;
  pitch = 512;
  roll = 512;
  aux1 = 0;
  aux2 = 0;
  //HC12_Data.switches = 0; //<---
}

/*
void resetHC12Payload() 
{
  HC12_Payload.lat = 0;
  HC12_Payload.lon = 0;
  HC12_Payload.heading = 0;
  HC12_Payload.pitch = 0;
  HC12_Payload.roll = 0;
  HC12_Payload.alt = 0;
  HC12_Payload.flags = 0;
}
*/

void HC12_Init() {
  resetHC12Data();
  //resetHC12Payload(); //<---
  HC12.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void HC12_Read_RC() {
  /*
  HC12_Payload.lat = 35.62;
  HC12_Payload.lon = 139.68;
  HC12_Payload.heading = att.heading;
  HC12_Payload.pitch = att.angle[PITCH];
  HC12_Payload.roll = att.angle[ROLL];
  HC12_Payload.alt = alt.EstAlt;
  memcpy(&HC12_Payload.flags, &f, 1); // first byte of status flags
  */
	
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
      //Serial.print(numDelimiter); //<---
      //Serial.print("\t"); //<---
      if(numDelimiter == 5){
        //Serial.println(input); //<---
        lastReceiveTime = millis();
      
        boundLow = input.indexOf(delimiter);
        throttle = input.substring(0, boundLow).toInt();
      
        boundHigh = input.indexOf(delimiter, boundLow+1);
        yaw = input.substring(boundLow+1, boundHigh).toInt();
      
        boundLow = input.indexOf(delimiter, boundHigh+1);
        pitch = input.substring(boundHigh+1, boundLow).toInt();

        boundHigh = input.indexOf(delimiter, boundLow+1);
        roll = input.substring(boundLow+1, boundHigh).toInt();

        boundLow = input.indexOf(delimiter, boundHigh+1);
        aux1 = input.substring(boundHigh+1, boundLow).toInt();
    
        aux2 = input.substring(boundLow+1).toInt();

        HC12_rcData[THROTTLE] = map(throttle, 0, 1023, 1000, 2000);
        HC12_rcData[ROLL] = map(yaw, 0, 1023, 1000, 2000);
        HC12_rcData[PITCH] = map(pitch, 0, 1023, 1000, 2000);
        HC12_rcData[YAW] = map(roll, 0, 1023, 1000, 2000);
        HC12_rcData[AUX1] = map(aux1, 0, 1, 2000, 1000);
        HC12_rcData[AUX2] = 1000;
      }   
    }
    currentTime = millis();
    if (currentTime - lastReceiveTime > 1000) {
      digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println("Connection Lost"); //<---
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      HC12.println("OK");
    }
    lastRunTime = millis();
  } 
}
#endif
