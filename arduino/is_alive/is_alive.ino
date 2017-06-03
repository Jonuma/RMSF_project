#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); /* RX:D2, TX:D3 */

void setup() {

  // Inicializa ambas as portas
  Serial.begin(9600);
  mySerial.begin(9600);
}
 
void loop() {
  
  // Ve se tem sinal na mySerial e manda para a Serial
  boolean bool1 = mySerial.available();
  /*Serial.print("mySerial: ");
  Serial.println(bool1);*/
  if (bool1) {
    int inByte = mySerial.read();
    Serial.write(inByte);
  }
 
  // Analogamente, ve se tem sinal na Serial, e manda para mySerial
  boolean bool2 = Serial.available();
  /*Serial.print("Serial: ");
  Serial.println(bool2);*/
  if (bool2) {
    int inByte = Serial.read();
    mySerial.write(inByte);
  }
  delay(1000);
}
