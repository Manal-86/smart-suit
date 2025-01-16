#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <MLX90614.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  Serial.println("Beat!");
}


void setup() {
  Serial.begin(115200);
  lcd.init();  // initialize the lcd
  lcd.init();
  lcd.backlight();
  Serial.println(F("Adafruit MLX90614 test"));
  mlx.begin();
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println(F("FAILED"));
    for (;;)
      ;
  } else {
    Serial.println(F("SUCCESS"));
  }

  Serial.print(F("Initializing pulse oximeter.."));

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}


void loop() {
  Serial.print(F("Ambient = "));
  Serial.print(mlx.readAmbientTempC());
  Serial.print(F("*C\tObject = "));
  Serial.print(mlx.readObjectTempC());
  Serial.println(F("*C"));
  Serial.print(F("Ambient = "));
  Serial.print(mlx.readAmbientTempF());
  Serial.print(F("*F\tObject = "));
  Serial.print(mlx.readObjectTempF());
  Serial.println(F("*F"));
  Serial.println();
  //ON LCD I2C
  lcd.setCursor(0, 0);
  lcd.print(F("A= "));
  lcd.setCursor(0, 2);
  lcd.print(mlx.readAmbientTempC());
  lcd.setCursor(0, 6);
  lcd.print(F("O= "));
  lcd.setCursor(0, 8);
  lcd.print(mlx.readObjectTempC());
  delay(1000);

  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print(F("H.R:"));
    Serial.print(pox.getHeartRate());
    Serial.print(F("bpm/SpO2:"));
    Serial.print(pox.getSpO2());
    Serial.println(F("%"));

    lcd.setCursor(0, 1);
    lcd.print("H.R:");
    lcd.setCursor(4, 1);
    lcd.print(pox.getHeartRate());
    lcd.setCursor(10, 1);
    lcd.print("O2:");
    lcd.setCursor(14, 1);
    lcd.print(pox.getSpO2());
    //lcd.setCursor(16, 1);
    //lcd.println("%");
    tsLastReport = millis();
  }
}
