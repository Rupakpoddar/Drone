#include <Servo.h>
#define MIN_THROTTLE 1000
#define MAX_THROTTLE 2000

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ESC1.attach(3, MIN_THROTTLE, MAX_THROTTLE);
  ESC2.attach(9, MIN_THROTTLE, MAX_THROTTLE);
  ESC3.attach(10, MIN_THROTTLE, MAX_THROTTLE);
  ESC4.attach(11, MIN_THROTTLE, MAX_THROTTLE);
}

void loop() {
  ESC1.write(180);
  ESC2.write(180);
  ESC3.write(180);
  ESC4.write(180);

  // Turn ON the ESCs when LED_BUILTIN is HIGH
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);

  ESC1.write(0);
  ESC2.write(0);
  ESC3.write(0);
  ESC4.write(0);
}
