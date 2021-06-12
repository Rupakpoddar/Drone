/*
 * Written by Rupak Poddar
 * https://youtu.be/T0L7FtrbVBs
 * https://github.com/Rupakpoddar/Drone
 */

#include <NeoSWSerial.h>
#define PADDING 10
#define THROTTLE_INC 11

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
int aux2 = -1;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
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
      //Serial.print(numDelimiter);
      //Serial.print("\t");
      if(numDelimiter == 5){
        //Serial.println(input);
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
      aux2 = -1;
      Serial.println("\nConnection Lost\n");
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      HC12.println("OK");
    }
    lastRunTime = millis();
    Serial.print(throttle);
    Serial.print(",");
    Serial.print(yaw);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.print(roll);
    Serial.print(",");
    Serial.print(aux1);
    Serial.print(",");
    Serial.println(aux2);
  }
}
