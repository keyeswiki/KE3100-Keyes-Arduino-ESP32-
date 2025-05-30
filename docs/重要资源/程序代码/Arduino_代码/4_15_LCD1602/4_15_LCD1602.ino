/*
 * 文件名 : 4_15_LCD1602
 * 功能   : LCD1602显示
 * 编译IDE：ARDUINO 2.3.5
 * 作者   : https://www.keyesrobot.cn/
*/

#include <LiquidCrystal_I2C.h>

//初始化LCD1602，其中0x27为I2C地址
//I2C接口:SDA引脚接SDA(IO21), SCL引脚接SCL(IO22)
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  //LCD初始化
  lcd.init();
  // 打开/关闭（可选）背光
  lcd.backlight();
  //lcd.noBacklight();

  //设置位置
  lcd.setCursor(0, 0);
  //显示
  lcd.print("HELLO WORLD 0");
  lcd.setCursor(0, 1);
  lcd.print("HELLO WORLD 1");

  //清空显示
  // lcd.clear();
}

void loop() {

  // Turn the display on/off (quickly)
  //lcd.noDisplay();
  //lcd.display();

  // Turns the underline cursor on/off
  //lcd.noCursor();
  //lcd.cursor();

  // Turn on and off the blinking cursor
  // lcd.noBlink();
  // lcd.blink();

  // These commands scroll the display without changing the RAM
  //lcd.scrollDisplayLeft();
  //lcd.scrollDisplayRight();

  // This is for text that flows Left to Right
  //lcd.leftToRight();
  //lcd.rightToLeft();

  // This will 'right justify' text from the cursor
  //lcd.autoscroll();
  //lcd.noAutoscroll();
}
