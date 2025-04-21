/*
 * 文件名 : Light-Control-System
 * 功能   : 结合光敏传感器和LED模拟光控系统
 * 编译IDE：ARDUINO 2.3.3
 * 作者   : https://www.keyestudio.com/
*/

#define PhotocecllPin 34  //定义光敏传感器引脚
#define LED           27  //定义LED引脚

void setup() {
  //初始化串口
  Serial.begin(9600);
  //设置光敏引脚为输入模式
  pinMode(PhotocecllPin,INPUT);
  //设置LED引脚为输出模式
  pinMode(LED,OUTPUT);
}

void loop() {
  //读取光敏传感器的值
  int ReadValue = analogRead(PhotocecllPin);
  //打印值，注意：ESP32开发板是12-bit的ADC，读取到的值范围：0~4095
  Serial.print("Photocecll value: ");
  Serial.println(ReadValue);
  //判断
  //光敏传感器的值 >= 800时，关闭LED
  //光敏传感器的值 < 800时，打开LED
  if(ReadValue >= 800) {
    digitalWrite(LED,LOW);
    Serial.println("LED OFF");
  }
  else{
    digitalWrite(LED,HIGH);
    Serial.println("LED ON");
  }
  delay(100);
}
