void setup() {
  Serial.begin(9600);
  String s = "CacaOK";
  if(s.indexOf("tK") > 0)
    Serial.println("yeih!");;
}

void loop() {
  // put your main code here, to run repeatedly:

}
