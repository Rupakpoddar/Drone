#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "bitmap.h"
#define LEDBUILTIN 13
#define THROTTLE A0
#define YAW A1
#define PITCH A2
#define ROLL A3
#define AUX1 5
#define AUX2 6

SoftwareSerial HC12(3, 2);
Adafruit_SSD1306 display(128, 64);

unsigned long lastReceiveTime = 0;

int throttle = 512;
int yaw = 512;
int pitch = 512;
int roll = 512;
int aux1 = 1;
int aux2 = -1;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.setFont(&FreeMonoBold9pt7b);
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
}

void loop() {
  // Analog Read
  throttle = analogRead(THROTTLE);
  yaw = 1023 - analogRead(YAW);
  pitch = analogRead(PITCH);
  roll = 1023 - analogRead(ROLL);
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
  /*
  display.clearDisplay();
  display.setCursor(0,15);
  display.print("T:");
  display.print(throttle);
  display.print(" ");
  display.print("Y:");
  display.print(yaw);
  display.print("\n");
  display.print("P:");
  display.print(pitch);
  display.print(" ");
  display.print("R:");
  display.print(roll);
  display.print("\n");
  display.print("A1:");
  display.print(aux1);
  display.print(" ");
  display.print("A2:");
  display.print(aux2);
  display.display();
  */
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
