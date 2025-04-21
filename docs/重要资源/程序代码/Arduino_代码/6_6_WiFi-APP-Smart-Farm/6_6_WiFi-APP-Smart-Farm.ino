/*
 * 文件名 : 6_6_WiFi-APP-Smart-Farm
 * 功能   : WiFi+APP控制智慧农场管理系统
 * 编译IDE：ARDUINO 2.3.5
 * 作者   : https://www.keyesrobot.cn/
*/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <dht11.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include "BuzzerMusic.h"

//显示
#define DHT11PIN        17  //温湿度传感器引脚
#define RAINWATERPIN    35  //雨滴传感器引脚
#define LIGHTPIN        34  //光敏传感器引脚
#define WATERLEVELPIN   33  //水位传感器
#define SOILHUMIDITYPIN 32  //土壤湿度传感器
//控制
#define LEDPIN          27  //LED引脚
#define RELAYPIN        25  //继电器水泵控制引脚
#define SERVOPIN        26  //舵机引脚
#define FANPIN1         19  //风扇的IN+引脚
#define FANPIN2         18  //风扇的IN-引脚
#define BUZZERPIN       16  //蜂鸣器引脚

/*替换为您的网络凭据（输入您自己的WiFi名称和密码）*/
const char* ssid = "REPLACE_WITH_YOUR_SSID";  //输入你自己的WiFi的名称
const char* password = "REPLACE_WITH_YOUR_PASSWORD"; //输入你自己的WiFi密码

//初始化LCD1602，其中0x27为I2C地址
LiquidCrystal_I2C lcd(0x27,16,2);
WiFiServer server(80);  //初始化wifi端口
dht11 DHT11;            //初始化温湿度传感器
Servo myservo;          // create servo object to control a servo
                // 16 servo objects can be created on the ESP32

//定义变量，存储传感器值
String request;
String dataBuffer;
int Temperature;   //温度
int Humidity;      //湿度
int SoilHumidity;  //土壤湿度
int Light;         //光照亮度
int WaterLevel;    //水位
int Rainwater;     //雨量

void setup() {
  Serial.begin(9600);
  //连接到wifi
  WiFi.begin(ssid, pwd);
  //判断是否连接成功
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  delay(1000);
  //串口打印wifi名称和IP地址
  Serial.println("Connected to WiFi");
  Serial.print("WiFi NAME:");
  Serial.println(ssid);
  Serial.print("IP:");
  Serial.println(WiFi.localIP());

  //LCD初始化
  lcd.init();
  // Turn the (optional) backlight off/on
  lcd.backlight();
  //lcd.noBacklight();
  lcd.clear();
  //设置位置
  lcd.setCursor(0, 0);
  //显示
  lcd.print(ssid);
  //设置位置
  lcd.setCursor(0, 1);
  //显示
  lcd.print(WiFi.localIP());
    
  //引脚初始化
  pinMode(LEDPIN,OUTPUT);
  pinMode(RAINWATERPIN,INPUT);
  pinMode(LIGHTPIN,INPUT);
  pinMode(SOILHUMIDITYPIN,INPUT);
  pinMode(WATERLEVELPIN,INPUT);
  pinMode(RELAYPIN,OUTPUT);
  pinMode(FANPIN1,OUTPUT);
  pinMode(FANPIN2,OUTPUT);
  pinMode(BUZZERPIN,OUTPUT);
  delay(1000);

  // attaches the servo on pin 26 to the servo object
  myservo.attach(SERVOPIN);

  //启动服务
  server.begin();
}

void loop() {
  //用于检查是否有客户端连接到Web服务器。
  //当客户端连接到服务器时，server.available()方法将返回一个WiFiClient对象，用于与客户端通信。
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      //判断服务器是否传来数据
      if (client.available()) {
        request = client.readStringUntil('s');
        Serial.print("Received message: ");
        Serial.println(request);
      }
        //获取所有传感器的数据
        getSensorsData();
        //把多个数据拼接到dataBuffer中
        dataBuffer = "";
        dataBuffer += String(Temperature,HEX);
        dataBuffer += String(Humidity,HEX);
        dataBuffer += dataHandle(SoilHumidity);
        dataBuffer += dataHandle(Light);
        dataBuffer += dataHandle(WaterLevel);
        dataBuffer += dataHandle(Rainwater);
        //发送数据到服务器，传给APP
        client.print(dataBuffer);
        delay(500);

      //LED
      if(request == "a")
      {
        digitalWrite(LEDPIN,HIGH);
      }
      else if(request == "A")
      {
        digitalWrite(LEDPIN,LOW);
      }
      //灌溉
      else if(request == "b")
      {
        digitalWrite(RELAYPIN,HIGH);
        delay(400);//灌溉的延时
        digitalWrite(RELAYPIN,LOW);
        delay(650);
      }
      //风扇
      else if(request == "c")
      {
        delay(800);
        digitalWrite(FANPIN1, HIGH);
        digitalWrite(FANPIN2, LOW);
        delay(200);
      }
      else if(request == "C")
      {
        digitalWrite(FANPIN1, LOW);
        digitalWrite(FANPIN2, LOW);
      }
      //饲料盒
      else if(request == "D")
      {
        //舵机转到180度时，打开饲料盒
        myservo.write(180);
        delay(500);
      }
      else if(request == "d")
      {
        //舵机转到80度时，关闭饲料盒
        myservo.write(80);
      }
      //音乐
      else if(request == "e")
      {
        Music();
      }
      request = "";
    }
    Serial.println("Client disconnected");
  }
}

void getSensorsData() {
  //获取数据
  int chk = DHT11.read(DHT11PIN);
  //雨滴传感器
  Rainwater = analogRead(RAINWATERPIN);
  //光敏传感器
  Light = analogRead(LIGHTPIN);
  //土壤湿度传感器
  SoilHumidity = analogRead(SOILHUMIDITYPIN) * 1.8;
  //水位传感器
  WaterLevel = analogRead(WATERLEVELPIN) * 1.8;
  //温度
  Temperature = DHT11.temperature;
  //湿度
  Humidity = DHT11.humidity;
}

void Music() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 98; thisNote++) {
  
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations2[thisNote];
    tone(BUZZERPIN, melody2[thisNote],noteDuration);
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZERPIN);
  }
}

//将数据转换成百分比
String dataHandle(int data){
  // 将模拟值转换为百分比
  int percentage = (data / 4095.0) * 100;
  percentage = percentage > 100 ? 100 : percentage;
  // 6个字符用于存储十六进制字符串，1个字符用于终止符
  char hexString[3];
  // 将十六进制值转换成6位十六进制字符串，并添加前导零
  sprintf(hexString, "%02X", percentage);

  return hexString;
}