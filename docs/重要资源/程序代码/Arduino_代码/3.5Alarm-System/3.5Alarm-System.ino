/*
 * 文件名 : Alarm-System
 * 功能   : 结合人体红外热释传感器和无源蜂鸣器模拟防盗警报系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#define BuzzerPin 16        //蜂鸣器模块连接引脚为16
#define PyroelectricPIN 23  //人体红外热释传感器引脚为23
#define Led 27              //led引脚为27

void setup() {
  Serial.begin(9600);
  //设置引脚的模式
  pinMode(BuzzerPin,OUTPUT);
  pinMode(PyroelectricPIN,INPUT);
  pinMode(Led,OUTPUT);
}
void loop() {
  //读取传感器值
  int ReadValue = digitalRead(PyroelectricPIN);
  if(ReadValue){
    Serial.println("Someone");
    digitalWrite(Led,HIGH);
    //警报声
    for(int i = 200; i<=1000; i+=10){ 
      tone(BuzzerPin,i);
      delay(10);
    }
    digitalWrite(Led,LOW);
    //警报声
    for(int i = 1000; i>=200; i-=10){ 
      tone(BuzzerPin,i);
      delay(10);
    }
  }
  //关闭蜂鸣器
  noTone(BuzzerPin);
  Serial.println("No one");
}