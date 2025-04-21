/*
 * 文件名 : Intelligent-Feeding-System
 * 功能   : 结合超声波传感器和舵机模拟智能喂养系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#include <ESP32_Servo.h>  //导入ESP32开发板的舵机库
Servo myservo;  // 创建舵机对象来控制舵机
                // 在ESP32上可以创建16个舵机对象

#define TrigPin 12 //trig接D12
#define EchoPin 13 //echo接D13
#define ServoPin 26 //舵机的引脚
int duration,distance;

void setup(){

  Serial.begin(9600); //设置波特率为9600
  pinMode(TrigPin,OUTPUT);  //trig设置为输出模式
  pinMode(EchoPin,INPUT);   //echo设置为输入模式
  myservo.attach(ServoPin);   // 将引脚26上的舵机连接到伺服对象上
}
void loop(){
  Serial.println(getDistance());
  //当超声波检测动物的距离，达到2~7cm时，打开饲料盒，否则关闭
  if (getDistance() >= 2 && 7 >= getDistance()) {
    //舵机转到80度时，打开饲料盒
    myservo.write(80);
    delay(500);
  }
  else{
    myservo.write(180);
    delay(500);
  }
}

//把获取超声波的检测距离，写在一个方法里
float getDistance() {

  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);	//给trig至少10us的高电平时间触发
  digitalWrite(TrigPin,LOW);
  duration = pulseIn(EchoPin,HIGH);	//echo高电平的时间
  distance = duration/58;		//换算成距离，单位为cm
  delay(50);
  
  return distance;
}