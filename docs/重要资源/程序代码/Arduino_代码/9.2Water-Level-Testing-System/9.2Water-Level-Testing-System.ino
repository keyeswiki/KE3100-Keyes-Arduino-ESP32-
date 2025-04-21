/*
 * 文件名 : Water-Level-Testing-System
 * 功能   : 结合水位传感器，LCD1602和无源蜂鸣器模拟水位监测系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#include <LiquidCrystal_I2C.h>

#define BuzzerPin 16  //蜂鸣器的引脚
#define WaterLevelPin 33  //水位传感器的引脚

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {

  //串口初始化
  Serial.begin(9600);
  //设置水位传感器引脚输入模式
  pinMode(WaterLevelPin,INPUT);
  //设置无源蜂鸣器引脚输出模式
  pinMode(BuzzerPin,OUTPUT);

  //LCD初始化
  lcd.init();
  //开启LCD背光
  lcd.backlight();
  //清除LCD上显示的信息
  lcd.clear();
}

void loop() {
  //读取传感器的值
  int ReadValue = analogRead(WaterLevelPin);
  //设置显示位置
  lcd.setCursor(0, 0);
  //设置显示信息
  lcd.print("WaterLevel:");
  lcd.setCursor(6, 1);
  lcd.print(ReadValue);
  
  //当检测的水位高于阈值时，打开蜂鸣器预警
  if(ReadValue >= 2000)
  {
    tone(BuzzerPin,659);
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