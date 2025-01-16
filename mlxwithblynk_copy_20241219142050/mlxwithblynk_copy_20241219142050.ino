/*************************************************************
  This is a simple demo of sending and receiving some data.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6XIYRiEx9"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "U5VuS_sLoHkLrLLYontBBcFfoqh-nAeZ"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address, 16 chars, 2 lines
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Wi-Fi credentials
char ssid[] = "Engineering Village New";
char pass[] = "TodayIsFriday2022";

BlynkTimer timer;

// Virtual Pin V0 Handler

void sendSensor() {
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = ");
  Serial.print(mlx.readObjectTempC());
  Serial.println("*C");

  lcd.setCursor(0, 0);
  lcd.print("A= ");
  lcd.print(mlx.readAmbientTempC());

  lcd.setCursor(0, 1);
  lcd.print("O= ");
  lcd.print(mlx.readObjectTempC());

  float A = mlx.readAmbientTempC();
  float O = mlx.readObjectTempC();  // or dht.readTemperature(true) for Fahrenheit


  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, A);
  Blynk.virtualWrite(V2, O);
}








void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  mlx.begin();
  lcd.init();
  lcd.backlight();
  Serial.println("Adafruit MLX90614 test");
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
