/*
 * 文件名 : Temperature-Control-System
 * 功能   : 结合温湿度传感器和电机模拟农场温控系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#include <LiquidCrystal_I2C.h>
#include <dht11.h>

#define DHT11PIN 17 //温湿度传感器的引脚
#define MotorPin1 19  //(IN+)
#define MotorPin2 18  //(IN-)

dht11 DHT11;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.backlight();  

  pinMode(MotorPin1,OUTPUT);
  pinMode(MotorPin2,OUTPUT);

  //设置PWM输出，可进行电机调速

  ledcAttach(MotorPin1, 1200, 8); //设置MotorPin1频率为1200，PWM分辨率为8，即占空比256.
  ledcAttach(MotorPin2, 1200, 8); //设置MotorPin2频率为1200，PWM分辨率为8，即占空比256.
}

void loop() {
  //定义两个温湿度值
  int Temperature;
  int Humidity;
  //获取数据
  int chk = DHT11.read(DHT11PIN);

  Temperature = DHT11.temperature;
  Humidity = DHT11.humidity;
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(Temperature); 

  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.setCursor(5, 1);
  lcd.print(Humidity);
  delay(500);
  
  if (Temperature >= 25) {
    //左转
    ledcWrite(MotorPin1, 100);
    ledcWrite(MotorPin2, 0);
  }else{
    //停止
    delay(3000);
    ledcWrite(MotorPin1, 0);
    ledcWrite(MotorPin2, 0);
    delay(200);
  }
}