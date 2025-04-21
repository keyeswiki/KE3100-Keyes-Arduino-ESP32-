/*
 * 文件名 : Passive-Buzzer
 * 功能   : 让无源蜂鸣器发生
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#define BuzzerPin 16  //定义蜂鸣器引脚

void setup() {
  //设置为输出模式
  pinMode(BuzzerPin,OUTPUT);
}

void loop() {
  digitalWrite(BuzzerPin,HIGH);
  delayMicroseconds(500);//延时500us
  digitalWrite(BuzzerPin,LOW);
  delayMicroseconds(500);//延时500us
}
