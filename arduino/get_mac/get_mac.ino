#include <WiFiClient.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <ESP8266.h>


void setup() {
  String clientMac = "";
  unsigned char mac[6];
  WiFi.macAddress(mac);
  clientMac += macToStr(mac);

}

void loop() {
  // put your main code here, to run repeatedly:

}

String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
  result += String(mac[i], 16);
  if (i < 5)
  result += ':';
  }
  return result;
}

/*********** OOOOUUUUUUUUU ***************/


/*
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);
 
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  //AT command to get the MAC address (station mode)
  sendData("AT+CIPSTAMAC?\r\n", 2000, DEBUG);
  Serial.println("** Final **");
}
 
void loop() {}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
 */
