#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <SoftwareSerial.h>
#include "bitmap.h"
#define LEDBUILTIN 13
#define THROTTLE A0
#define YAW A1
#define PITCH A2
#define ROLL A3
#define AUX1 5
#define AUX2 6

SoftwareSerial HC12(2, 3);
Adafruit_SSD1306 display(128, 64);

unsigned long lastReceiveTime = 0;

struct Data_Package {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte aux1;
  byte aux2;
};

struct Return_Package {
  byte APIN0;
  byte APIN1;
  byte APIN2;
};

Data_Package packet; //Create a variable with the above structure
Return_Package return_data;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.setFont(&FreeMonoBold9pt7b);
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setTextWrap(true);
  display.drawBitmap(0, 0, bootlogo, 128, 64, WHITE);
  display.display();
  delay(500);  

  pinMode(THROTTLE, INPUT);
  pinMode(YAW, INPUT);
  pinMode(PITCH, INPUT);
  pinMode(ROLL, INPUT);
  pinMode(AUX1, INPUT_PULLUP);
  pinMode(AUX2, INPUT_PULLUP);
  Serial.begin(9600);
  HC12.begin(9600);
  delay(500);

  packet.throttle = 127;
  packet.yaw = 127;
  packet.pitch = 127;
  packet.roll = 127;
  packet.aux1 = 1;
  packet.aux2 = 1;
}

void loop() {
  packet.throttle = map(analogRead(THROTTLE), 0, 1023, 0, 255);
  packet.yaw = map(analogRead(YAW), 0, 1023, 255, 0);
  packet.pitch = map(analogRead(PITCH), 0, 1023, 0, 255);
  packet.roll = map(analogRead(ROLL), 0, 1023, 255, 0);
  packet.aux1 = digitalRead(AUX1);
  packet.aux2 = digitalRead(AUX2);
  HC12.write((byte*)&packet, sizeof(Data_Package));
  
  display.clearDisplay();
  display.setCursor(0,15);
  display.print("T:");
  display.print(packet.throttle);
  display.print(" ");
  display.print("Y:");
  display.print(packet.yaw);
  display.print("\n");
  display.print("P:");
  display.print(packet.pitch);
  display.print(" ");
  display.print("R:");
  display.print(packet.roll);
  display.print("\n");
  display.print("A1:");
  display.print(packet.aux1);
  display.print(" ");
  display.print("A2:");
  display.print(packet.aux2);
  display.display();
  delay(30);  // Important delay

  if(HC12.available() > 0){
    HC12.readBytes((byte*)&return_data, sizeof(Return_Package));
    lastReceiveTime = millis();
    Serial.print((int)return_data.APIN0);
    Serial.print("\t");
    Serial.print(return_data.APIN1);
    Serial.print("\t");
    Serial.println(return_data.APIN2);
    digitalWrite(LEDBUILTIN, LOW);
  }
  unsigned long currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) {
    digitalWrite(LEDBUILTIN, HIGH);
    Serial.println("Connection Lost");
    delay(50);
  }
}
