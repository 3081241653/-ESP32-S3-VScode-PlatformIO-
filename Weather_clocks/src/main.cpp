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
