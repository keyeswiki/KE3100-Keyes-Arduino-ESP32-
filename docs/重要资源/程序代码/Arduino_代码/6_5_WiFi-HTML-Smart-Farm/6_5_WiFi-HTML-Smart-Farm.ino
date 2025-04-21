/*
 * 文件名 : 6_5_WiFi-HTML-Smart-Farm
 * 功能   : WiFi网页控制智慧农场管理系统
 * 编译IDE：ARDUINO 2.3.5
 * 作者   : https://www.keyesrobot.cn/
*/

#include <Arduino.h>
/* 判断是ESP32还是8266开发板，两种开发板的库不通用，在编译之前引入对应的开发板的库，以免编译出错 */
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
/* 导入相应的库文件 */
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
#include <ESP32Servo.h>

#define DHT11PIN        17  //温湿度传感器引脚
#define LEDPIN          27  //LED引脚
#define SERVOPIN        26  //舵机引脚
#define FANPIN1         19  //风扇的IN+引脚
#define FANPIN2         18  //风扇的IN-引脚
#define STEAMPIN        35  //雨滴传感器引脚
#define LIGHTPIN        34  //光敏传感器引脚
#define SOILHUMIDITYPIN 32  //土壤湿度传感器
#define WATERLEVELPIN   33  //水位传感器
#define RELAYPIN        25  //继电器引脚

dht11 DHT11;
//初始化LCD1602，其中0x27为I2C地址
LiquidCrystal_I2C lcd(0x27,16,2);

/*替换为您的网络凭据（输入您自己的WiFi名称和密码）*/
const char* ssid = "REPLACE_WITH_YOUR_SSID";  //输入你自己的WiFi的名称
const char* password = "REPLACE_WITH_YOUR_PASSWORD"; //输入你自己的WiFi密码

static int A = 0;
static int B = 0;
static int C = 0;

// 创建WebServer对象, 端口号80，使用端口号80可以直接输入IP访问，使用其它端口需要输入IP:端口号访问
AsyncWebServer server(80);
Servo myservo;  // create servo object to control a servo
                // 16 servo objects can be created on the ESP32

// 一个储存网页的数组
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<title>TEST HTML ESP32</title>
<head>
  <meta charset="utf-8">
</head>
<body>
  <div class="btn">
    <div id="dht"></div>
    <button id="btn-led" onclick="setLED()">LED</button>
    <button id="btn-fan" onclick="setFan()">Fan</button>
    <button id="btn-feeding" onclick="setFeeding()">Feeding</button>
    <button id="btn-watering" onclick="setWatering()">Watering</button>
  </div>
</body>
<script>
    // 按下按钮会运行这个JS函数
    function setLED() {
      var payload = "A"; // 需要发送的内容
      // 通过get请求给 /set
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/set?value=" + payload, true);
      xhr.send();
    }
    function setFan() {
      var payload = "B"; // 需要发送的内容
      // 通过get请求给 /set
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/set?value=" + payload, true);
      xhr.send();
    }
    function setFeeding() {
      var payload = "C"; // 需要发送的内容
      // 通过get请求给 /set
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/set?value=" + payload, true);
      xhr.send();
    }
    function setWatering() {
      var payload = "D"; // 需要发送的内容
      // 通过get请求给 /set
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/set?value=" + payload, true);
      xhr.send();
    }
    // 设置一个定时任务, 1000ms执行一次
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          // 此代码会搜索ID为dht的组件，然后使用返回内容替换组件内容
          document.getElementById("dht").innerHTML = this.responseText;
        }
      };
      // 使用GET的方式请求 /dht
      xhttp.open("GET", "/dht", true);
      xhttp.send();
    }, 1000)
</script>
<style>
  /*网页效果*/
  html,body{margin: 0;width: 100%;height: 100%;}
  body{display: flex;justify-content: center;align-items: center;}
  #dht{text-align: center;width: 100%;height: 100%;color: #fff;background-color: #47a047;font-size: 48px;}
  .btn button{width: 100%;height: 100%;border: none;font-size: 30px;color: #fff;position: relative;}
  button{color: #ffff;background-color: #89e689;margin-top: 20px;}
  .btn button:active{top: 2px;}
</style>
</html>
)rawliteral";

//获取数据，并将数据打包成HTML格式的数据
String Merge_Data(void)
{
  //定义变量，存储值
  String dataBuffer;
  String Humidity;
  String Temperature;
  String Steam;
  String Light;
  String SoilHumidity;
  String WaterLevel;
  //获取数据
  int chk = DHT11.read(DHT11PIN);
  //雨滴传感器
  Steam = String(analogRead(STEAMPIN) / 4095.0 * 100);
  //光敏传感器
  Light = String(analogRead(LIGHTPIN));
  //土壤湿度传感器
  int shvalue = analogRead(SOILHUMIDITYPIN) / 4095.0 * 100 * 2.3;
  shvalue = shvalue > 100 ? 100 : shvalue;
  SoilHumidity = String(shvalue);
  //水位传感器
  int wlvalue = analogRead(WATERLEVELPIN) / 4095.0 * 100 * 2.5;
  wlvalue = wlvalue > 100 ? 100 : wlvalue;
  WaterLevel = String(wlvalue);
  //温度
  Temperature = String(DHT11.temperature);
  //湿度
  Humidity = String(DHT11.humidity);
  
  // 将数据打包为一个HTML显示代码
  dataBuffer += "<p>";
  dataBuffer += "<h1>Sensor Data</h1>";
  dataBuffer += "<b>Temperature:</b><b>";
  dataBuffer += Temperature;
  dataBuffer += "</b><b>℃</b><br/>";
  dataBuffer += "<b>Humidity:</b><b>";
  dataBuffer += Humidity;
  dataBuffer += "</b><b>%rh</b><br/>";
  dataBuffer += "<b>WaterLevel:</b><b>";
  dataBuffer += WaterLevel;
  dataBuffer += "</b><b>%</b><br/>";
  dataBuffer += "<b>Steam:</b><b>";
  dataBuffer += Steam;
  dataBuffer += "</b><b>%</b><br/>";
  dataBuffer += "<b>Light:</b><b>";
  dataBuffer += Light;
  dataBuffer += "</b><b></b><br/>";
  dataBuffer += "<b>SoilHumidity:</b><b>";
  dataBuffer += SoilHumidity;
  dataBuffer += "</b><b>%</b><br/>";
  dataBuffer += "</p>";

  // 最后要将数组返回出去
  return dataBuffer;
}

// 下发处理回调函数
void Config_Callback(AsyncWebServerRequest *request)
{
  if (request->hasParam("value")) // 如果有值下发
  {
    // 获取下发的数据
    String HTTP_Payload = request->getParam("value")->value();
    // 打印调试信息   
    Serial.printf("[%lu]%s\r\n", millis(), HTTP_Payload.c_str());

    //LED
    if(HTTP_Payload == "A"){
      if(A){
        digitalWrite(LEDPIN,LOW);
        A = 0;
      }
      else{
        digitalWrite(LEDPIN,HIGH);
        A = 1;
      }
    }
    //FAN
    if(HTTP_Payload == "B"){
      if(B){
        //停止
        digitalWrite(FANPIN1, LOW);
        digitalWrite(FANPIN2, LOW);
        B = 0;
      }
      else{
        delay(500);
        digitalWrite(FANPIN1, HIGH);
        digitalWrite(FANPIN2, LOW);
        delay(500);
        B = 1;
      }
    }
    //FEEDING
    if(HTTP_Payload == "C"){
      if(C){
        //舵机转到80度时，打开饲料盒
        myservo.write(80);
        delay(500);
        C = 0;
      }
      else{
        C = 1;
        //舵机转到180度时，关闭饲料盒
        myservo.write(180);
        delay(500);
      }
    }
    //WATERING
    if(HTTP_Payload == "D"){
      digitalWrite(RELAYPIN,HIGH);
      delay(400);//灌溉的延时
      digitalWrite(RELAYPIN,LOW);
      delay(650);
    }
  }
  request->send(200, "text/plain", "OK"); // 发送接收成功标志符
}

//设置访问无效的网址入口
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
  
void setup()
{
  Serial.begin(9600);
  // 连接到热点，并通过LCD屏显示IP地址
  WiFi.begin(SSID, PASS);
  while (!WiFi.isConnected())
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

  
  //引脚初始化
  pinMode(LEDPIN,OUTPUT);
  pinMode(STEAMPIN,INPUT);
  pinMode(LIGHTPIN,INPUT);
  pinMode(SOILHUMIDITYPIN,INPUT);
  pinMode(WATERLEVELPIN,INPUT);
  pinMode(RELAYPIN,OUTPUT);
  pinMode(FANPIN1,OUTPUT);
  pinMode(FANPIN2,OUTPUT);

  delay(1000);

  // attaches the servo on pin 26 to the servo object
  myservo.attach(SERVOPIN);   

  //LCD初始化
  lcd.init();
  // Turn the (optional) backlight off/on
  lcd.backlight();
  //lcd.noBacklight();
  //清空显示
  lcd.clear();
  
  
  //设置位置
  lcd.setCursor(0, 0);
  //显示
  lcd.print("IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  
  // 添加HTTP主页，当访问的时候会把网页推送给访问者
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html); });

  // 设置反馈的信息，在HTML请求这个Ip/dht这个链接时，返回打包好的传感器数据
  server.on("/dht", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", Merge_Data().c_str()); });

  // 绑定配置下发的处理函数
  server.on("/set", HTTP_GET, Config_Callback);   
  // 绑定好访问的无效地址
  server.onNotFound(notFound);
  // 初始化HTTP服务器
  server.begin();  
}
void loop(){

}