#ifndef __OLED_FONT_H
#define __OLED_FONT_H

#include "stm32f10x.h"

// 8x8 ASCII可视字符字模（0x20~0x7E）
extern const uint8_t OLED_Font8x8[][8];

// 测试打印用的图片
extern const uint8_t OLED_Img[128 * 64];

typedef struct Chinese_Font {
    const uint8_t *Chinese_Index;
    const uint8_t Chinese_Word[32];
} Chinese_Model;

extern const Chinese_Model Chinese[4];

#endif