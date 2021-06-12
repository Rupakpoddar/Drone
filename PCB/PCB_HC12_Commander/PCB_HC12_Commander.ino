#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);
#include <Fonts/FreeMonoBold9pt7b.h>
#include <SoftwareSerial.h>
#define SET 4
#define TX 3
#define RX 2
#define LEDBUILTIN 13
unsigned char n = 0;

SoftwareSerial mySerial(TX, RX);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.setFont(&FreeMonoBold9pt7b);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setCursor(35,35);
  display.print("RUPAK");
  display.display();
  
  pinMode(SET, OUTPUT);
  pinMode(LEDBUILTIN, OUTPUT);
  digitalWrite(SET, LOW);
  delay(1000);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop(){
  if (n == 0)
  mySerial.write("AT");

  if (n == 1)
  mySerial.write("AT+DEFAULT");
/*
  if (n == 2)
  mySerial.write("AT+C010");

  if (n == 3)
  mySerial.write("AT+FU3");

  if (n == 4)
  mySerial.write("AT+P8");
 */
  if (mySerial.available()){
    display.clearDisplay();
    display.setCursor(0,15);
    display.print(String(mySerial.readString()));
    display.display();
    n++;
    delay(1000);
  }
 /*
  if (mySerial.available()){
    Serial.write(mySerial.read());
  }
  if (Serial.available()){
    mySerial.write(Serial.read());
  }
 */
}
