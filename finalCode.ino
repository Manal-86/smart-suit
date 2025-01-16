#define BLYNK_TEMPLATE_ID "TMPL60Sl_UJTp"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "lJ_iECaP8j5DoFEkg_9xjtCaDxifD8Ok"

#define BLYNK_PRINT Serial
#define RXD2 16
#define TXD2 17

#define Red_LED 32
#define Green_LED 34

#define REPORTING_PERIOD_MS 15000  // MAX30100
#define PERIOD_MS 5000

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <DFRobot_MLX90614.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "manal iPhone";  // wifi name
char pass[] = "abc12345";       // wifi password

BlynkTimer timer;
PulseOximeter pox;
DFRobot_MLX90614_I2C sensor;         // instantiate an object to drive our sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
uint32_t tsLastReport = 0;
uint32_t LastReport = 0;
float hr, o2, temp;
const int emgPin = 35;  // Analog pin connected to EMG sensor
int LED = 19;
unsigned long t = millis();
int emgValue;
String sendAT(String s, unsigned int t2 = 3000) {
  Serial.println(s);
  Serial2.println(s);
  s = "";
  unsigned long t1 = millis();
  while (millis() - t1 < t2) {
    while (Serial2.available()) {
      s += char(Serial2.read());
    }
  }
  Serial.println(s);
  Serial.println(s.length());
  return s;
}
void onBeatDetected() {
  Serial.println(F("Beat!"));
}
void myTimerEvent() {  // You can send any value at any time.// Please don't send more that 10 values per second.
  String response = sendAT("AT+LOCATION=2");
  if (millis() - t > 300000UL) {
    sendAT("AT+CMGF=1");
    sendAT("AT+CMGS=\"96777303\"");
    Serial2.printf("https://maps.google.com/maps/place/%s", response.substring(17, 37));
    Serial2.println((char)26);
    sendAT("AT");
    t = millis();
  }
  hro2();
  Temp();
  Serial.print(F("ht:"));
  Serial.print(hr);
  Serial.print(F("\to2:"));
  Serial.print(o2);
  Serial.print(F("\ttemp:"));
  Serial.print(temp);
  emgValue = analogRead(emgPin);
  Serial.println(emgValue);
  Blynk.virtualWrite(V2, hr);
  Blynk.virtualWrite(V1, o2);
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V0, emgValue);
}

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);  // Setup a function to be called every second
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(Red_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  sendAT("AT");
  sendAT("ATI");
  // sendAT("ATD99999999", 10000);
  // sendAT("ATH");
  sendAT("AT+GPS=1");
  lcd.init();  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("wLCM 2 BIOMETRIC"));
  lcd.setCursor(0, 1);
  lcd.print(F(" ONE AT A TIME! "));
  delay(5000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
}
void hro2() {
  lcd.setCursor(0, 0);
  lcd.print(F("Measuring Hr/SpO2"));
  lcd.setCursor(0, 1);
  lcd.print(F("Put ur fngr on ox"));
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Hrate: "));
  lcd.setCursor(0, 1);
  lcd.print(F("SpO2: "));
  Serial.print(F("Initializing pulse oximeter.."));
  if (!pox.begin()) {
    Serial.println(F("FAILED"));
    for (;;)
      ;
  } else {
    Serial.println(F("SUCCESS"));
  }
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
  tsLastReport = millis();
  while (millis() - tsLastReport < REPORTING_PERIOD_MS) {
    pox.update();
    while (millis() - LastReport > 1000UL) {
      hr = pox.getHeartRate();
      o2 = pox.getSpO2();
      Serial.print(hr);
      Serial.print("Heart rate:");
      Serial.print("bpm / SpO2:");
      Serial.print(o2);
      Serial.println("%");
      lcd.setCursor(7, 0);
      lcd.print(hr);
      lcd.print(F("bpm   "));
      lcd.setCursor(7, 1);
      lcd.print(o2);
      lcd.print(F("%   "));
      LastReport = millis();
    }
  }
  if (((hr < 60) || (hr > 120)) || (o2 < 94)) {
    digitalWrite(Red_LED, HIGH);
    sendAT("AT");
    sendAT("ATI");
    sendAT("AT+CMGF=1");
    sendAT("AT+CMGS=\"96777303\"");
    Serial2.printf("There is An Emergency");
    sendAT("ATD96777303", 15000);
    Serial2.println((char)26);
    //sendAT("AT");
    sendAT("ATH");
    digitalWrite(Red_LED, LOW);
  } else {
    digitalWrite(Red_LED, LOW);
    digitalWrite(Green_LED, HIGH);
  }
}
void Temp() {
  while (NO_ERR != sensor.begin()) {
    Serial.println(F("Communication with device failed, please check connection"));
    delay(3000);
  }
  Serial.println(F("Begin ok!"));
  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);

  lcd.setCursor(0, 0);
  lcd.print(F("Measuring Temp"));
  lcd.setCursor(0, 1);
  lcd.print(F("pnt snsr 2 head"));
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));

  tsLastReport = millis();
  while (millis() - tsLastReport < PERIOD_MS) {
    // Make sure to call update as fast as possible
    temp = sensor.getObjectTempCelsius();
    while (millis() - LastReport > 1000UL) {
      Serial.print(F("Object celsius : "));
      Serial.print(temp);
      Serial.println(F(" °C"));
      lcd.setCursor(6, 0);
      lcd.print(temp);
      lcd.print(F(" C   "));
      LastReport = millis();
    }
  }
  if ((temp > 37.2) || (temp < 35.0) || (emgValue >= 5000)) {
    digitalWrite(Red_LED, HIGH);
    sendAT("AT");
    sendAT("ATI");
    sendAT("AT+CMGF=1");
    sendAT("AT+CMGS=\"96777303\"");
    Serial2.printf("There is An Emergency");
    sendAT("ATD96777303", 15000);
    Serial2.println((char)26);
    //sendAT("AT");
    sendAT("ATH");
    digitalWrite(Red_LED, LOW);
  } else {
    digitalWrite(Red_LED, LOW);
    digitalWrite(Green_LED, HIGH);
  }
}
