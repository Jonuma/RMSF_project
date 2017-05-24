/*********** OOOOUUUUUUUUU ***************/
#include <SoftwareSerial.h>

//RX pino 3, TX pino 2
SoftwareSerial esp8266(3, 2);

#define SSID        "falafel_p9"
#define PASSWORD    "test12345"
#define DEBUG true
#define CH_PD 4
 
void setup()
{
  /*pinMode(CH_PD, OUTPUT);
  digitalWrite(CH_PD, HIGH);*/
  Serial.begin(9600);
  esp8266.begin(9600);
  //AT command to get the MAC address (station mode)
  sendData("AT\r\n", 2000, DEBUG);
  delay(200);
  sendData("AT+CIPSTAMAC?\r\n", 2000, DEBUG);
  delay(200);
  sendData("AT+CWMODE=1\r\n", 2000, DEBUG);
  delay(200);
  String command = "AT+CWJAP=\""+String(SSID)+"\",\""+String(PASSWORD)+"\"\r\n";
  sendData(command, 2000, DEBUG);
  delay(200);
  sendData("AT+CWJAP?\r\n", 2000, DEBUG);
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
