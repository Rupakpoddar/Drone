#include <SoftwareSerial.h>

SoftwareSerial HC12(2, 3);

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

Data_Package packet;
Return_Package return_data;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  delay(500);
}

void loop() {
  if(HC12.available() > 0){
    HC12.readBytes((byte*)&packet, sizeof(Data_Package));
    lastReceiveTime = millis();
    Serial.print((int)packet.throttle);
    Serial.print("\t");
    Serial.print(packet.yaw);
    Serial.print("\t");
    Serial.print(packet.pitch);
    Serial.print("\t");
    Serial.print(packet.roll);
    Serial.print("\t");
    Serial.print(packet.aux1);
    Serial.print("\t");
    Serial.println(packet.aux2);
  }
  unsigned long currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) {
    Serial.println("Connection Lost");
    delay(50);
  }
  else{
    return_data.APIN0 = map(analogRead(A0), 0, 1023, 0, 255);
    return_data.APIN1 = map(analogRead(A1), 0, 1023, 0, 255);
    return_data.APIN2 = map(analogRead(A2), 0, 1023, 0, 255);
    HC12.write((byte*)&return_data, sizeof(Return_Package));
    delay(30);
  }
}
