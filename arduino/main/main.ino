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
#define SSID        "falafel_p9"
#define PASSWORD    "test12345"
/*Base de dados*/
#define user "78094"
//inputs
#define CH_PD 4
#define RST 5
#define GPIO0 6
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
String mac = "";
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

  delay(1000);
  
  if(wifi.kick())
    Serial.println("wifi module OK!");
  else
    Serial.println("wifi module NOT OK!");
    
  delay(1000);
  
  if(wifi.setOprToStation())
    Serial.println("Station OK.");
  else
    Serial.println("Station ERROR");
  
  while(!wifi.joinAP(SSID, PASSWORD)){
      Serial.println("Falha na conexao AP.");       
  }
  Serial.println("Successful connection");
  //Prints IP and MAC of esp8266
  String ip_mac = wifi.getLocalIP().c_str();
  Serial.println(ip_mac);
  //Gets MAC of esp8266
  
  int idx = find_text("MAC,", ip_mac);
  for(int i = idx+5 ; i < idx+22 ; i++){
    mac += ip_mac.charAt(i); 
  }
  

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
  String data = String("temperature=")+(int)temp_read+"&light="+(int)lux+"&moisture="+(int)hum_read+"&idArduino="+mac;
  const char data_c[data.length()];
  data.toCharArray(data_c, data.length());
  String get_message = "GET /ist1"+String(user)+"/RMSF/server.php?"+data+" HTTP/1.1\r\n" + "Host: web.ist.utl.pt\r\n" + "\r\n";
  
  //TODO checks se a data foi bem enviada
  delay(500);
  //Sends GET request to server
  sendData("AT+CIPSEND="+String(get_message.length())+"\r\n", 2000, DEBUG, 100);
  //sendData(get_message, 5000, DEBUG, 5000);

  /******* A MAO***************==============================================*****/
  //String response = "";
  mySerial.print(get_message);
  int timeout = 5000;
  long int time = millis();
  int i=0;
  while ( (time + timeout) > millis())
  {
    if (mySerial.available())
      {
        // The esp has data so display its output to the serial window
        char c = mySerial.read(); // read the next character.
//        response += c;
        Serial.print(c);
        i++;
     }
       
  }
   /*Serial.print("Response = ");
   Serial.println(response.length());
   Serial.println("");
   Serial.print(response);*/
   Serial.println("\n==\n");



  delay(5000);

  //Response of server (configuration of user)
  int wait_time = 20000; //5 minutes
  //Waits for response during wait_time
  int value = recvDigit(wait_time, 1000, "\"config\":");
  Serial.println(value);
  switch(value){
    case -1:
    Serial.println("Did not receive value from server"); break;
    case 0:
    Serial.println("Configuration stays the same"); break;
    default:
    Serial.println("Configuration changed!"); break;
  }

  //Closes TCP session
  sendData("AT+CIPCLOSE\r\n", 5000, DEBUG, 100);
  Serial.println("\nConnection released");

  delay(100000);
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


boolean sendData(String command, const int timeout, boolean debug, const int interval)
{
  // Envio dos comandos AT para o modulo
  int dif = 0;
  String response = "";
  mySerial.print(command);
  //delay(interval);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    /*int aval= mySerial.available();
    Serial.print("Available = ");
    Serial.println(aval);*/
    while (mySerial.available())
    {
      // The esp has data so display its output to the serial window
      char c = mySerial.read(); // read the next character.
      response += c;
      /*if((response.length()-dif) > 30){
        delay(1000);
        dif += 30;
      }*/
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return (response.indexOf("OK") > 0);
}

boolean sendDataX(String command, const int timeout, boolean debug, const int interval)
{
  // Envio dos comandos AT para o modulo
  int readCount = 0;
  char response[1024] = {0};
  mySerial.print(command);
  //delay(interval);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    /*int aval= mySerial.available();
    Serial.print("Available = ");
    Serial.println(aval);*/
    while (mySerial.available())
    {
      // The esp has data so display its output to the serial window
      readCount += mySerial.readBytes(response, mySerial.available()); // read the next character.
      //response += c;
      /*if((response.length()-dif) > 30){
        delay(1000);
        dif += 30;
      }*/
    }
  }
  if (debug)
  {
    Serial.print("Response = ");
    Serial.println(readCount);
    Serial.print(response);
    /*for(int i=0; i < response.length(); i++){
      Serial.print(i);
      Serial.print(" -> ");
      Serial.print(response.charAt(i));
      Serial.print(" ");
    }*/
  }
  return true;//(response.indexOf("OK") > 0);
}

boolean checkACK(){
  return Serial.find("OK");
}

int recvDigit(const int timeout, const int interval, char key[])
{
  String response = "";
  boolean received = false;
  int value=-1;
  long int time = millis();
  while ( (time + timeout) > millis() && !received)
  {
    while (mySerial.available())
    {
      /*Serial.println("Entrou");
      delay(100);*/    
      // The esp has data so display its output to the serial window
      char c = mySerial.read(); // read the next character.
      response += c;
     /*
      if(mySerial.find("+IPD,")){
        // Delay to wait for the buffer to fill up
        delay(interval);
        mySerial.find(key);
        value = mySerial.read()-48; // read the digit associated with key, -48 to pass from ASCII code to digit value
        received = true;
        Serial.println("Entrou!");
      }*/
    }
    
  }
  Serial.print(response);
  
  return value;
}

int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

/*
void concatToChar(char *line, String s, int size){
  char line[size];
  s.toCharArray(line, size);
}*/
