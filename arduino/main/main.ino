#include <SparkFunTSL2561.h>
#include <ESP8266.h>
#include <doxygen.h>
#include <dht11.h>
#define HUM_PIN A0
#define TEMP_PIN 3 //digital input
  /*  Humidity sensor:
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water        */
  
/* WiFi macros*/
#define SSID        "NOME_DA_SUA_REDE_WIFI"
#define PASSWORD    "senha_da_sua_rede"
//inputs
#define CH_PD 4
#define RST 5
#define GPIO0 6

ESP8266 wifi(Serial);

void setup() {
  // Comunicates with pc and WIFi module at 9600 bit/s
  Serial.begin(9600);
  //waits one second after power up
  while(millis() < 1000);

  pinMode(TEMP_PIN, INPUT);
  /*
  pinMode(CH_PD,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(GPIO0,OUTPUT);
  digitalWrite(CH_PD,HIGH);
  digitalWrite(RST,HIGH); 
  digitalWrite(GPIO0,HIGH);

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
}

void loop() {
 //Hum sensor
  int hum_read = 0;

  hum_read = analogRead(HUM_PIN);
  delay(100);

// Temp sensor
  dht11 DHT11;
  int chk = DHT11.read(TEMP_PIN);
  float temp_read = 0;

  switch (chk)
  {
    case DHTLIB_OK: 
      Serial.println("OK");
      temp_read = DHT11.temperature; 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
      break;
    default: 
      Serial.println("Unknown error"); 
      break;
  }

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 1);
  Serial.print("Humidity: ");
  Serial.println(hum_read);

  delay(2000);
  
}
