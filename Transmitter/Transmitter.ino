/*
 * Written by Rupak Poddar
 * https://youtu.be/T0L7FtrbVBs
 * https://github.com/Rupakpoddar/Drone
 */

#include <Adafruit_SSD1306.h>
#include <NeoSWSerial.h>
#include "bitmap.h"
#define LEDBUILTIN 13
#define THROTTLE A0
#define YAW A1
#define PITCH A2
#define ROLL A3
#define AUX1 5
#define AUX2 6
#define PADDING 10

NeoSWSerial HC12(3, 2);
Adafruit_SSD1306 display(128, 64);

unsigned long lastReceiveTime = 0;
int meanVal[4];

int throttle = 512;
int yaw = 512;
int pitch = 512;
int roll = 512;
int aux1 = 1;
int aux2 = -1;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.drawBitmap(0, 0, bootlogo, 128, 64, WHITE);
  display.display();
  delay(1000);

  pinMode(THROTTLE, INPUT);
  pinMode(YAW, INPUT);
  pinMode(PITCH, INPUT);
  pinMode(ROLL, INPUT);
  pinMode(AUX1, INPUT_PULLUP);
  //pinMode(AUX2, INPUT_PULLUP);
  Serial.begin(9600);
  HC12.begin(9600);
  delay(100);

  // Calibrate Joysticks
  for(int i = 0; i < 4; i++){
    int sum = 0;
    for(int j = 0; j < 5; j++){
      if(i == 0){
        sum += analogRead(THROTTLE);
      }
      else if(i == 1){
        sum += analogRead(YAW);
      }
      else if(i == 2){
        sum += analogRead(PITCH);
      }
      else{
        sum += analogRead(ROLL);
      }
    }
    meanVal[i] = sum/5;
    if( i == 1 || i == 3){
      meanVal[i] = 1023 - meanVal[i]; 
    }
  }
}

void loop() {
  // Analog Read
  throttle = analogRead(THROTTLE);
  if(throttle < (meanVal[0] - PADDING)){
    throttle = map(throttle, 0, meanVal[0], 0, 512);
  }
  else if(throttle > (meanVal[0] + PADDING)){
    throttle = map(throttle, meanVal[0], 1023, 512, 1023);
  }
  else{
    throttle = 512;
  }
  
  yaw = 1023 - analogRead(YAW);
  if(yaw < (meanVal[1] - PADDING)){
    yaw = map(yaw, 0, meanVal[1], 0, 512);
  }
  else if(yaw > (meanVal[1] + PADDING)){
    yaw = map(yaw, meanVal[1], 1023, 512, 1023);
  }
  else{
    yaw = 512;
  }
  
  pitch = analogRead(PITCH);
  if(pitch < (meanVal[2] - PADDING)){
    pitch = map(pitch, 0, meanVal[2], 0, 512);
  }
  else if(pitch > (meanVal[2] + PADDING)){
    pitch = map(pitch, meanVal[2], 1023, 512, 1023);
  }
  else{
    pitch = 512;
  }
  
  roll = 1023 - analogRead(ROLL);
  if(roll < (meanVal[3] - PADDING)){
    roll = map(roll, 0, meanVal[3], 0, 512);
  }
  else if(roll > (meanVal[3] + PADDING)){
    roll = map(roll, meanVal[3], 1023, 512, 1023);
  }
  else{
    roll = 512;
  }
  
  aux1 = digitalRead(AUX1);

  // Transmit
  HC12.print(throttle);
  HC12.print(",");
  HC12.print(yaw);
  HC12.print(",");
  HC12.print(pitch);
  HC12.print(",");
  HC12.print(roll);
  HC12.print(",");
  HC12.print(aux1);
  HC12.print(",");
  HC12.print(aux2);
  HC12.println("");

  // Display
  display.clearDisplay();
  display.fillRect(0, map(throttle, 0, 1023, display.height(), 0), display.width()/12, display.height(), SSD1306_INVERSE);
  display.fillRect(2*display.width()/12, map(yaw, 0, 1023, display.height(), 0), display.width()/12, display.height(), SSD1306_INVERSE);
  display.fillRect(4*display.width()/12, map(pitch, 0, 1023, display.height(), 0), display.width()/12, display.height(), SSD1306_INVERSE);
  display.fillRect(6*display.width()/12, map(roll, 0, 1023, display.height(), 0), display.width()/12, display.height(), SSD1306_INVERSE);
  display.fillRect(8*display.width()/12, map(aux1, 0, 1, display.height(), 0), display.width()/12, display.height(), SSD1306_INVERSE);
  display.drawRect(10*display.width()/12, 0, display.width()/12, display.height(), SSD1306_INVERSE);
  display.display();
 
  delay(50);  // Important delay

  if(HC12.available() > 0){
    String State = HC12.readStringUntil('\n');
    Serial.println(State);
    lastReceiveTime = millis();
    digitalWrite(LEDBUILTIN, LOW);
  }
  unsigned long currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) {
    digitalWrite(LEDBUILTIN, HIGH);
    Serial.println("Connection Lost");
    delay(100);
  }
}
