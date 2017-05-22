#include <SparkFunTSL2561.h>
#include <Wire.h> //communication via I2C
#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <doxygen.h>
#include <dht11.h>
#define HUM_PIN A0
#define TEMP_PIN 7 //digital input
  /*  Humidity sensor:
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water        */

  /*      Light Sensor     
  # Default integration: 402ms
  # Default gain:        low (1X)   */
  
/* WiFi macros*/
#define SSID        "NOME_DA_SUA_REDE_WIFI"
#define PASSWORD    "senha_da_sua_rede"
//inputs
#define CH_PD 4
#define RST 5
#define GPIO0 6
/* Light Sensor pins:
                     SDA    SCL
Uno, Redboard, Pro    A4     A5
*/


SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */
//WiFi object
ESP8266 wifi(mySerial);
//LightSensor object
SFE_TSL2561 light;
//Temperature object
dht11 DHT11;

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned char time; // Integration time, 0 = 13.7ms, 1 = 101m, 2 = 402ms, 3 = manual integration (manualStart/manualStop)
unsigned int ms;  // Integration ("shutter") time in milliseconds

void setup() {
  // Comunicates with pc and WIFi module at 9600 bit/s
  Serial.begin(9600);
  mySerial.begin(9600);
  //waits one second after power up
  while(millis() < 1000);

  pinMode(TEMP_PIN, INPUT);
  
  pinMode(CH_PD,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(GPIO0,OUTPUT);
  digitalWrite(CH_PD,HIGH);
  digitalWrite(RST,HIGH); 
  digitalWrite(GPIO0,HIGH);
/*
  if(wifi.kick())
    Serial.println("wifi module OK!");
  else
    Serial.println("wifi module NOT OK!");

  if(wifi.setOprToStation())
    Serial.println("Station OK.");
  else
    Serial.println("Station ERROR");

  if (wifi.joinAP(SSID, PASSWORD)){
      Serial.println("Successful connection");
      Serial.print("IP: ");
      Serial.println(wifi.getLocalIP().c_str());    
  }else{
      Serial.println("Falha na conexao AP.");
  }
*/
  light.begin();
  gain = 0;
  time = 2;
  light.setTiming(gain,time,ms); //changes the parameters
  //ms is setted with the integration time
  light.setPowerUp();
}

void loop() {
//Humidity sensor
  int hum_read = -1;

  hum_read = analogRead(HUM_PIN);
  delay(100);

// Temperature sensor
  int chk = DHT11.read(TEMP_PIN);
  float temp_read = -1;

  switch (chk){
    case DHTLIB_OK: 
      Serial.println("OK");
      temp_read = DHT11.temperature; break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); break;
    default: 
      Serial.println("Unknown error"); 
  }

// Luminosity sensor
  delay(ms); // delay during integration time

  unsigned int data0, data1;
  boolean check_light = light.getData(data0,data1);
  double lux=-1;    // Resulting lux value
  boolean good; // To check if lux measure is saturated
  if (check_light){
    // Perform lux calculation: (returns: 1 - if successfull, 0 - if infrared/visible light sensor was saturated
    good = light.getLux(gain,ms,data0,data1,lux);
  }

  /*boolean check_light = true;
  double lux=-1;    // Resulting lux value
  boolean good; // To check if lux measure is saturated*/
// Printing Results
  printResults( temp_read, hum_read, check_light, lux, good);
  

  delay(2000);
  
}

void printLightError(byte error){
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error){
    case 0:
      Serial.println("success");break;
    case 1:
      Serial.println("data too long for transmit buffer");break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");break;
    case 3:
      Serial.println("received NACK on data");break;
    case 4:
      Serial.println("other error");break;
    default:
      Serial.println("unknown error");
  }
}

void printResults(float temp_read, int hum_read, boolean check_light, double lux, boolean good){
  Serial.print("Temperature (°C): ");
  Serial.println((float)temp_read, 1);
  Serial.print("Humidity: ");
  Serial.println(hum_read);
  if (check_light){
    Serial.print("Luminosity (lux): ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }else{
    // getData() returned false because of an I2C error
    byte error = light.getError();
    printLightError(error);
  }
}

