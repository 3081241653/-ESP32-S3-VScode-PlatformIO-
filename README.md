# Weather_clocks简单天气站项目
本项目使用 VScode、PlatformIO 和 ESP32 来实现一个简单的天气站。该天气站能够获取当前的温度和湿度，并且通过 OLED 屏幕显示出来。
### 先叠点护甲
本人小白，基本啥也不会，代码也是GTP写的，但是从购买板子到功能实现的过程中有很多坑，希望新小白可以直接照着复现。
高手看个乐子就好，毕竟效果挺差的。

### 软件需求

- Visual Studio Code
- PlatformIO 插件

### 硬件需求

- 合宇ESP32-S3 开发板
- 0.96寸OLED 显示屏，驱动芯片SSD1306
- ![image](https://github.com/3081241653/Weather_clocks/assets/151439821/1ca52686-ff87-4e69-a863-24de54222bef)
- 0.96寸OLED 显示屏，驱动芯片SSD1306
- ![918bfb277422830f5073c946c9dd1f4](https://github.com/3081241653/Weather_clocks/assets/151439821/c0f5af27-ec98-4174-8be8-4f6f4d5f96bf)


### 安装
- 将板子和屏幕正确连接
- 在VScode的PlatformIO插件创好项目（网上有好多教程，这里就不多说了）关于板子型号的选择如下图，初次安装可能很慢，睡一觉就好了
- ![image](https://github.com/3081241653/Weather_clocks/assets/151439821/671498fb-96e7-4188-a55e-dfc4c47b3a35)



### 代码
- 打开项目后粘代码
- 在platformio.ini文件中改成下面的代码
```cpp
; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
board_build.arduino.partitions = default_16MB.csv
board_build.arduino.memory_type = qio_opi
build_flags = -DBOARD_HAS_PSRAM
board_upload.flash_size = 16MB
```
- 如图
- ![image](https://github.com/3081241653/Weather_clocks/assets/151439821/c341c0ae-6d96-48ce-9eb3-609dacdededf)
- 这几行代码当时安装库的时候出现的，好像可以不用
```cpp
lib_deps = 
olikraus/U8g2@^2.35.15
bblanchon/ArduinoJson@^7.0.4
```
- 在src的main.cpp文件中粘下面代码（代码粘好后先把库安装好，直接在Libraries安装就好,还要改点东西）
```cpp
#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SCL 12
#define SDA 11

// OLED 配置
#define OLED_RESET    U8X8_PIN_NONE  // 如果不使用复位功能，则可忽略

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /*clock=*/SCL, /*data=*/SDA, /*reset=*/OLED_RESET);

// WiFi 配置
const char* ssid = "QAZWSX";
const char* password = "123456789";

// API 配置
const char* apiEndpoint = "https://api.seniverse.com/v3/weather/now.json?key=SD4mEkAb4tN5rggbB&location=xian&language=zh-Hans&unit=c";

void setup() {
  Serial.begin(115200);
  
  // OLED 初始化
  u8g2.begin();

  // 连接 WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 发送HTTP请求并获取响应
  HTTPClient http;
  http.begin(apiEndpoint);
  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    // 如果请求成功，在屏幕上显示"6666"
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_mr);
    u8g2.setCursor(0, 15);
    u8g2.print("WiFi connected");
    u8g2.setCursor(0, 30);
    u8g2.print("IP: ");
    u8g2.print(WiFi.localIP());
    u8g2.setCursor(0, 45);
    u8g2.print("Data ok!");
    u8g2.sendBuffer();
    
    // 等待两秒
    delay(2000);
    
    // 解析JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, http.getString());

    // 提取并显示信息
    String location = doc["results"][0]["location"]["name"];
    String timezone = doc["results"][0]["location"]["timezone"];
    String text = doc["results"][0]["now"]["text"];
    String temperature = doc["results"][0]["now"]["temperature"];
    String lastUpdate = doc["results"][0]["last_update"];

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_mr);
    u8g2.setCursor(0, 10);
    u8g2.print("Location: xian ");
    u8g2.print(location);  // 使用变量打印位置信息
    u8g2.setCursor(0, 25);
    u8g2.print("Timezone: ");
    u8g2.print(timezone);  // 使用变量打印时区信息
    u8g2.setCursor(0, 40);
    u8g2.print("Temperature: ");
    u8g2.print(temperature);  // 使用变量打印温度信息
    u8g2.setCursor(0, 55);
    u8g2.print("time: ");
    u8g2.print(lastUpdate);  // 使用变量打印最后更新时间
    u8g2.sendBuffer();
  } else {
    // 如果请求失败，在屏幕上显示错误消息
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_mr);
    u8g2.setCursor(0, 15);
    u8g2.print("WiFi connected");
    u8g2.setCursor(0, 30);
    u8g2.print("IP: ");
    u8g2.print(WiFi.localIP());
    u8g2.setCursor(0, 45);
    u8g2.print("Data error!");
    u8g2.sendBuffer();
  }
  http.end();
}

void loop() {
  // 每隔一分钟访问一次
  delay(60000);
  setup(); //重新执行setup()以获取最新的天气信息
}
```
- 改API，这里用的是知心天气的,就下图的这个链接改成你自己的
- 链接获取
- ![image](https://github.com/3081241653/Weather_clocks/assets/151439821/864639a8-525e-4459-bc40-2765e6d92f93)




