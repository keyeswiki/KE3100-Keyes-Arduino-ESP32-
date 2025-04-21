/*
 * 文件名 : Lighting-System
 * 功能   : 结合按键和LED模拟照明系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#define ButtonPin 5   //定义按键引脚
#define LED       27  //定义LED引脚
int value = 0;        //定义一个判断按键的值

void setup() {
  //初始化串口并设置波特率为9600
  Serial.begin(9600);
  //设置引脚为输入模式
  pinMode(ButtonPin,INPUT);
  //设置引脚为输出模式
  pinMode(LED,OUTPUT);
}

void loop() {
  //定义一个值，存储读取到的按键值
  int ReadValue = digitalRead(ButtonPin); 
  //判断按钮是否被按下
  if (ReadValue == 0) {
    //软件去按键抖动
    delay(10);  
    if (ReadValue == 0) {
      value = !value;
      //判断按钮的状态，按一次为点亮LED，再按为熄灭LED，一直如此循环
      if(value) {
        digitalWrite(LED,HIGH);
      }else{
        digitalWrite(LED,LOW);
      }
    }
    //再次判断按钮，是否还处于按下状态
    //处于按下时：一直空循环，处于松开时：执行下一步
    while (digitalRead(ButtonPin) == 0); 
    delay(10);
  }
}
