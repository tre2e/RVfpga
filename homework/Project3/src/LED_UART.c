#if defined(D_NEXYS_A7)
#include <bsp_printf.h>
#include <bsp_mem_map.h>
#include <bsp_version.h>
#else
PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

// GPIO 地址
#define GPIO_SWs    0x80001400
#define GPIO_LEDs   0x80001404
#define GPIO_INOUT  0x80001408

// GPIO 读写宏
#define READ_GPIO(dir) (*(volatile unsigned *)dir)
#define WRITE_GPIO(dir, value) { (*(volatile unsigned *)dir) = (value); }

// 延时常数
#define DELAY 30000000

void DisplaySwitches(void)
{
    int switches_value = READ_GPIO(GPIO_SWs) >> 16; // 读取开关并右移
    WRITE_GPIO(GPIO_LEDs, switches_value); // 写入 LED
    int led_on = 0;
    for (int i = 0; i < 16; i++) {
        if (switches_value & (1 << i)) {
            printfNexys("LED%d 正在亮\n", i + 1);
            led_on = 1;
        }
    }
    if (!led_on) {
        printfNexys("无 LED 点亮\n");
    }
}

int main(void)
{
    int i;
    uartInit(); // 初始化串口
    WRITE_GPIO(GPIO_INOUT, 0xFFFF); // 配置 GPIO 输出
    printfNexys("程序开始：开关控制 LED\n");
    while (1) {
        DisplaySwitches();
        for (i = 0; i < DELAY; i++); // 延时
    }
    return 0;
}