/*
 * 文件名 : Auto-irrigation
 * 功能   : 结合水位传感器，土壤湿度传感器，继电器和水泵模拟自动灌溉系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#include <LiquidCrystal_I2C.h>

#define BuzzerPin 16  //定义蜂鸣器引脚
#define SoilHumidityPin 32  //定义土壤湿度传感器引脚
#define WaterLevelPin 33  //定义水位传感器引脚
#define RelayPin 25  //定义继电器引脚
#define ButtonPin 5 //定义按键引脚
int value = 0;      //定义一个判断按键的值

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  //设置引脚模式
  pinMode(BuzzerPin,OUTPUT);
  pinMode(SoilHumidityPin,INPUT);
  pinMode(WaterLevelPin,INPUT);
  pinMode(RelayPin,OUTPUT);
  pinMode(ButtonPin,INPUT);

  //LCD初始化
  lcd.init();
  //开启LCD背光
  lcd.backlight();
  //清除LCD上显示的信息
  lcd.clear();
}

void loop() {
  //定义变量，存储读取到的水位、湿度和按键信息
  int shvalue = analogRead(SoilHumidityPin);
  int wlvalue = analogRead(WaterLevelPin);
  int ReadValue = digitalRead(ButtonPin); 

  //设置显示位置
  lcd.setCursor(0, 0);
  //设置显示信息
  lcd.print("SoilHum:");
  lcd.setCursor(9, 0);
  lcd.print(shvalue);
  lcd.setCursor(0, 1);
  lcd.print("WaterLevel:");
  lcd.setCursor(11, 1);
  lcd.print(wlvalue);

  //判断按钮是否被按下
  if (ReadValue == 0) {
    //软件去按键抖动
    delay(10);  
    if (ReadValue == 0) {
      value = !value;
      Serial.print("The current status of the button is : ");
      Serial.println(value);
    }
    //再次判断按钮，是否还处于按下状态
    //按下：一直空循环，松开：执行下一步
    while (digitalRead(ButtonPin) == 0); 
  }
  //当检测湿度到低于设定的阈值时，打开蜂鸣器预警，可由按键进行关闭
  if(500 >= shvalue && value == 0)
  {
    tone(BuzzerPin,532);
    delay(100);
    tone(BuzzerPin,532);
    delay(100);
    tone(BuzzerPin,659);
    delay(100);
    noTone(BuzzerPin);  //关闭
  }
  //当检测水位到低于设定的阈值时，打开蜂鸣器预警，可由按键进行关闭
  if(500 >= wlvalue && value == 0)
  {
    tone(BuzzerPin,411);
    delay(100);
    tone(BuzzerPin,639);
    delay(100);
    tone(BuzzerPin,411);
    delay(100);
    noTone(BuzzerPin);  //关闭
  }
  //土壤湿度低于设定的阈值时，且水池里的水源充足，抽水系统会进行自动灌溉
  if(500 >= shvalue && wlvalue >= 1000) {
    digitalWrite(RelayPin,HIGH);
    delay(400);//灌溉的延时
    digitalWrite(RelayPin,LOW);
    delay(700);
  }
  delay(500);
  //清除显示
  lcd.clear();
}