#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }

  Serial.print("Initializing pulse oximeter..");

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
}


void loop() {
  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    //ON LCD I2C
    lcd.setCursor(0, 0);
    lcd.print("Heart rate:");
      lcd.setCursor(12, 0);
    lcd.print(pox.getHeartRate());
          lcd.setCursor(0, 1);
    lcd.print("bpm/SpO2:");
          lcd.setCursor(9, 1);
    lcd.print(pox.getSpO2());
          lcd.setCursor(14, 1);
    lcd.println("%");

    tsLastReport = millis();
  }
}
