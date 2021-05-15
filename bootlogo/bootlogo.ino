#include <Adafruit_SSD1306.h>
#include "bitmap.h"
Adafruit_SSD1306 display(128, 64);

void setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setTextWrap(true);  
  display.drawBitmap(0, 0, bootlogo, 128, 64, WHITE);
  display.display();
}

void loop(){
 
}
