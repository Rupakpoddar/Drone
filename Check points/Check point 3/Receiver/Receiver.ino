#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial HC12(2, 3);

unsigned long lastReceiveTime = 0, lastRunTime = 0;
const char delimiter = ',';
byte boundLow;
byte boundHigh;
String input;

int throttle = 512;
int yaw = 512;
int pitch = 512;
int roll = 512;
int aux1 = 1;
int aux2 = -1;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //delay(100);
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
        Serial.println(input);
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
      }   
    }
    currentTime = millis();
    if (currentTime - lastReceiveTime > 1000) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Connection Lost");
      //delay(100);
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      HC12.println("OK");
      //delay(100);
    }
    lastRunTime = millis();
  }
}
