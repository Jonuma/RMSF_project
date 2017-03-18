#include <dht11.h>
#define HUM_PIN A0
#define TEMP_PIN 3 //digital input
  /*  Humidity sensor:
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water        */

void setup() {
  // Comunicates with pc at 57600 bit/s
  Serial.begin(57600);

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

  delay(2000);
  
}
