void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("AT");
delay(1000);
Serial.println("ATI");
delay(1000);
Serial.println("ATD96777303;");
delay(25000);
Serial.println("ATH");
delay(1000);
Serial.println("AT+CMGF=1");
delay(1000);
Serial.println("AT+CMGS=\"96777303\"");
delay(1000);
Serial.println("TEST MESSAGE");
delay(1000);
Serial.println(char(26));
delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}