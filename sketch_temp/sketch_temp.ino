#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.display();
}

void loop() {
  for(int i = 0; i <= display.height(); i++){
    display.clearDisplay();
    display.fillRect(0 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.fillRect(2*display.width()/12 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.fillRect(4*display.width()/12 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.fillRect(6*display.width()/12 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.fillRect(8*display.width()/12 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.fillRect(10*display.width()/12 , display.height()-i, display.width()/12, display.height(), SSD1306_INVERSE); //x0, y0, x, y
    display.display();
    delay(100);
  }
}
