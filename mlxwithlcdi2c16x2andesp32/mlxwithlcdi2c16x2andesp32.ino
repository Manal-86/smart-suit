#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h> 
//#include <MLX90614.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
 lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();  
}

void loop() {
  Serial.print("Ambient = "); 
  Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); 
  Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); 
  Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); 
  Serial.print(mlx.readObjectTempF()); Serial.println("*F");
 
  Serial.println();

  lcd.setCursor(0,0);
  lcd.print("A= ");
    lcd.print(mlx.readAmbientTempC());
lcd.setCursor(1,1);
  lcd.print("O= ");
    lcd.print(mlx.readObjectTempC());
  delay(1000);
}