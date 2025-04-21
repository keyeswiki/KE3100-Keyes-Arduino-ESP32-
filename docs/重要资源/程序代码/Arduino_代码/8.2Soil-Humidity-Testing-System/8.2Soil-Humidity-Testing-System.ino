/*
 * 文件名 : Soil-Humidity-Testing-System
 * 功能   : 结合土壤湿度传感器和无源蜂鸣器模拟土壤湿度监测系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#include <LiquidCrystal_I2C.h>

#define BuzzerPin 16
#define SoilHumidityPin 32

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {

  pinMode(BuzzerPin,OUTPUT);
  pinMode(SoilHumidityPin,INPUT);

  lcd.init();
  lcd.backlight();  
  lcd.clear();

}

void loop() {

  float shvalue = analogRead(SoilHumidityPin);

  lcd.setCursor(0, 0);
  lcd.print("SoilHum:");
  lcd.setCursor(9, 0);
  lcd.print(shvalue);
  
  //当检测湿度到低于阈值时，打开蜂鸣器预警
  if(200 >= shvalue)
  {
    tone(BuzzerPin,532);
    delay(100);
    tone(BuzzerPin,532);
    delay(100);
    tone(BuzzerPin,659);
    delay(100);
    noTone(BuzzerPin);  //关闭
  }
  delay(500);
  lcd.clear();
}
