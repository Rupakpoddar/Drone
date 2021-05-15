#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);
#include <Fonts/FreeMonoBold9pt7b.h>

void setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.setFont(&FreeMonoBold9pt7b);
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setTextWrap(true);  

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  
  display.clearDisplay();
  display.setCursor(35,35);
  display.print("RUPAK");
  display.display();

  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void loop(){
  int x1, y1, x2, y2, s1, s2;
  x1 = analogRead(A0);
  y1 = analogRead(A1);
  x2 = analogRead(A2);
  y2 = analogRead(A3);
  s1 = digitalRead(5);
  s2 = digitalRead(6);

  display.clearDisplay();
  display.setCursor(0,15);
  display.print(x1);
  display.print("   ");
  display.print(y1);
  display.print("\n");
  display.print(x2);
  display.print("   ");
  display.print(y2);
  display.print("\n");
  display.print(s1);
  display.print("   ");
  display.print(s2);
  display.display();
}
