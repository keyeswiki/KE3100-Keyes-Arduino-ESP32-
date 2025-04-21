/*
 * 文件名 : 4_8_Read_PIR-Motion-Sensor
 * 功能   : 读取人体红外热释电传感器的值
 * 编译IDE：ARDUINO 2.3.5
 * 作者   : https://www.keyesrobot.cn/
*/

const int PyroelectricPIN = 23; // 将人体红外传感器的引脚连接到IO23

void setup() {
  Serial.begin(9600);  // 启动串口监视器，设置波特率为9600
  pinMode(PyroelectricPIN,INPUT); // 设置人体红外传感器的引脚为输入模式
}

void loop() {
  int ReadValue = digitalRead(PyroelectricPIN); //读取传感器值并将其赋值给变量ReadValue
  if(ReadValue){  //检测的人体运动时读取到高电平，并打印出相关的信息
    Serial.println("Someone");
  }
  else{  //未检测的人体运动时读取到高电平，并打印出相关的信息
    Serial.println("No one");
  }
  delay(100);
}