# Weather_clocks简单天气站项目
本项目使用 VScode、PlatformIO 和 ESP32 来实现一个简单的天气站。该天气站能够获取当前的温度和湿度，并且通过 OLED 屏幕显示出来。
### 先叠点护甲
本人小白，基本啥也不会，代码也是GTP写的，但是从购买板子到功能实现的过程中有很多坑，希望新小白可以直接照着复现。
高手看个乐子就好，毕竟效果挺差的。


### 硬件需求

- 合宇ESP32-S3 开发板
- 0.96寸OLED 显示屏，驱动芯片SSD1306

### 软件需求

- Visual Studio Code
- PlatformIO 插件

### 安装

克隆本仓库到本地计算机：

```bash
https://github.com/3081241653/Weather_clocks
``` 
### 代码


```cpp
#include <iostream>

int main() {
std::cout << "Hello, world!" << std::endl;
return 0;
}
```
