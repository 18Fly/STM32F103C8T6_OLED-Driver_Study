#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

int main()
{
    OLED_Init();
    OLED_ClearScreen();
    // OLED_ShowString(0, 0, "Hello World! I'm a STM32F103C8T6, nice to meet you all!");
    // OLED_ShowImage(0, 0, 128, 8, OLED_Img);
    OLED_ShowChinese(0, 0, "你好世界");
    // OLED_ShowImage(0, 0, 16, 2, Chinese[0].Chinese_Word);

    while (1) {
    }
}
