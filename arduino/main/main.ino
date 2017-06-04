#include <SparkFunTSL2561.h>
#include <Wire.h> //communication via I2C
#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <doxygen.h>
#include <dht11.h>
#define TIMEOUT 10 //minutes

  /*  Humidity sensor:
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water        */
#define HUM_PIN A0
#define DELTA_HUM 200
  /*      Light Sensor     
  # Default integration: 402ms
  # Default gain:        low (1X)   */
#define TEMP_PIN 7 //digital input  
/* WiFi macros*/
#define SSID        "falafel_p9"
#define PASSWORD    "test12345"
#define CHUNCK 48
#define CH_PD 4
/*Base de dados*/
#define USER "78094"
#define PATH "/RMSF/arduinoData.php"
/*Water Pump*/
#define MOTOR_PIN A1
#define BLINK 13

#define DEBUG true

/* Light Sensor pins:
                     SDA    SCL
Uno, Redboard, Pro    A4     A5
*/

//GPIO0 - amarelo, RST - azul
// TX - amarelo, RX - verde
SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */
//WiFi object
ESP8266 wifi(mySerial, 9600);
char mac[20]={0};
//LightSensor object
SFE_TSL2561 light;
//Temperature object
dht11 DHT11;
//Humidity values
int hum_target = 500;
int hum_thresh = hum_target - DELTA_HUM;

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned char time; // Integration time, 0 = 13.7ms, 1 = 101m, 2 = 402ms, 3 = manual integration (manualStart/manualStop)
unsigned int ms;  // Integration ("shutter") time in milliseconds

void setup() {
  // Comunicates with Serial port and WIFi module at 9600 bit/s
  Serial.begin(9600);
  mySerial.begin(9600);
  //waits one second after power up
  while(millis() < 1000);

  pinMode(TEMP_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BLINK, OUTPUT);
  pinMode(CH_PD,OUTPUT);

  digitalWrite(BLINK,LOW);
  digitalWrite(MOTOR_PIN,LOW);
  digitalWrite(CH_PD,HIGH);

  delay(1000);
  //Checks if wifi module is ready
  if(wifi.kick())
    Serial.println("wifi module OK!");
  else
    Serial.println("wifi module NOT OK!");
    
  delay(1000);
  
  //Sets wifi module to Station mode
  if(wifi.setOprToStation())
    Serial.println("Station OK.");
  else
    Serial.println("Station ERROR");

  //Joins to an AP
  while(!wifi.joinAP(SSID, PASSWORD)){
      Serial.println("Falha na conexao AP.");       
  }
  Serial.println("Successful connection");
  //Prints IP and MAC of esp8266
  String ip_mac = wifi.getLocalIP().c_str();
  Serial.println(ip_mac);
  
  //Gets MAC of esp8266  
  int idx = find_text("MAC,", ip_mac);
  int k=0;
  for( int i = idx+5 ; i < idx+22 ; i++){
    //mac += ip_mac.charAt(i);
    mac[k] = ip_mac.charAt(i);
    k++;
  }
  
  //Light sensor set up
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
      Serial.println("Temp OK");
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
  
//Prints results
  printResults( temp_read, hum_read, check_light, lux, good); 
  delay(200);


/*************************Connection***********************/
  //Opens TCP session with server
  while(!wifi.createTCP("web.ist.utl.pt", 80)){
    delay(200);
    Serial.println("Error connecting."); 
  }
  delay(200);  
  Serial.println("Connected to server.");

  //sets GET request
  char data[100];
  sprintf(data, "temperature=%d&light=%d&moisture=%d&idArduino=%s", (int)temp_read, (int)lux, (int)hum_read, mac);
  char get_message[100];
  sprintf(get_message, "GET /ist1%s%s?%s HTTP/1.1\r\nHost: web.ist.utl.pt\r\n\r\n", USER, PATH, data);

  delay(500);
  //Sends GET request to server
  sendData("AT+CIPSEND="+String(strlen(get_message))+"\r\n", 2000, DEBUG, 100, false, NULL);
  // Also gets congifuration from user
  int cfg = sendData(get_message, 5000, DEBUG, 5000, true, "\"config\":\"");

  //Response of server (configuration of user)
  switch(cfg){
    case -1:
    Serial.println("Did not receive value from server"); break;
    default:
    Serial.println("Configuration received!"); break;
  }

  //Closes TCP session
  sendData("AT+CIPCLOSE\r\n", 5000, DEBUG, 100, false, NULL);
  Serial.println("\nConnection released");
/*************************Connection[END]***********************/

  //Control of the Water Pump

  //Finds configuration of user
  switch(cfg){
    case 1:
    hum_target = 300; break;
    case 2:
    hum_target = 500; break;
    case 3:
    hum_target = 700; break;
    default:
    hum_target = 0;
    
  }
  // Threshold is DELTA_HUM lower than the target value
  hum_thresh = hum_target - DELTA_HUM;
  long int time = millis();
  long int timeout = TIMEOUT*60L*1000L;
  while((time + timeout) > millis()){
    hum_read = analogRead(HUM_PIN);
    checkHum(hum_read);
    delay(1000);
    Serial.print("Hum = ");
    Serial.println(hum_read);
    delay(1000);
  }
}

// Control of soil humidity follows an hysteresis
void checkHum(int hum_read){
    // It activates the bomb if below the threshold
    if(hum_read < hum_thresh){
      digitalWrite(BLINK,HIGH); //Debug LED
      digitalWrite(MOTOR_PIN,HIGH);
    }
    // It deactivates the bomb if above the target value
    if(hum_read >= hum_target){
      digitalWrite(BLINK,LOW);  //Debug LED
      digitalWrite(MOTOR_PIN,LOW);
    }
}

//Printing of light sensor error
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

//Prints the results
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

// Sends data to esp8266
// Returns the value associated with pattern
int sendData(String command, const int timeout, boolean debug, const int interval, boolean getValue, char * pattern)
{
  // Envio dos comandos AT para o modulo
  char response[CHUNCK+1]={0};  
  int i = 0;
  char c = 'n';
  int value = -1;
  //Sends command to module
  mySerial.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {

    while (mySerial.available())
    {
      if(!getValue){
        int aval = mySerial.available();
        //Reads data available at the module buffer
        if(mySerial.available() > CHUNCK){
           i = CHUNCK;
           mySerial.readBytes(response, CHUNCK);
        }else{
           i = aval;
           mySerial.readBytes(response, aval);
        };
        //Prints esp8266 response if debug set to true
        if (debug)
        {
          Serial.print(response);
        }
        for( int k = 0; k < CHUNCK+1 ; k++){
          response[k]='\0';
        }
      }else{
        //Finds a pattern in the module buffer, if getValue is set to true
        if(mySerial.find(pattern)){
          delay(500);//waits for buffer to fill up
          c = mySerial.read(); // read the digit associated with key
          value = c - 48; // -48 to pass from ASCII code to digit value
        }
      }      
    }
  }
  return value;
}

//Finds a substring (needle) in a string (haystack)
//Returns the fir occurence of the first character of needle
int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

