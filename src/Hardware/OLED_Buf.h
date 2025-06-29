/**
 * @file OLED_Buf.h
 * @author 18Fly (iphoneios@88.com)
 * @brief 使用硬件I2C与SSD1306芯片通讯，驱动OLED屏幕，使用缓冲区
 * @version 0.2
 * @date 2025-06-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __OLED_BUF_H
#define __OLED_BUF_H

#include "stm32f10x.h"
#include "OLED_Font.h"
#include "Delay.h"

#define OLED_ADDRESS      0x78

#define WAIT_EVENT(event) while (!I2C_CheckEvent(I2C1, event))

// 显示缓冲区，默认初始化全为0
extern uint8_t OLED_Buf[8][128];

void OLED_Init(void);

/**
 * @brief 写入命令
 *
 * @param Command 命令
 */
void OLED_WriteCommand(uint8_t Command);

/**
 * @brief 写入数据
 *
 * @param Data 数据
 */
void OLED_WriteData(uint8_t *Data);

/**
 * @brief 设置显示位置
 *
 * @param X 列
 * @param Page 页
 */
void OLED_SetPoint(uint8_t X, uint8_t Page);

/**
 * @brief 清屏
 *
 */
void OLED_ClearScreen(void);

/**
 * @brief 在指定位置显示字符
 *
 * @param X 列(由于一个字符8*8,所以列数为128/8=16列,范围0~15)
 * @param Page 页(共8行,范围0~7)
 * @param Char 显示的字符对应的ASCII码值(默认会减去32个前部分的不可显示字符,用于在字模库中作为行索引下标)
 */
void OLED_ShowChar(uint8_t X, uint8_t Page, uint8_t Char);

/**
 * @brief 在指定位置显示字符串
 *
 * @param X 列(由于一个字符8*8,所以列数为128/8=16列,范围0~15)
 * @param Page 页(共8行,范围0~7)
 * @param String 显示的字符串
 */
void OLED_ShowString(uint8_t X, uint8_t Page, char *String);

/**
 * @brief 在指定位置为原点绘制图形
 *
 * @param X 列(由于一个字符8*8,所以列数为128/8=16列,范围0~15)
 * @param Page 页(共8行,范围0~7)
 * @param Width 图片的宽度
 * @param Height 图片的高度
 * @param Image 图片取模信息
 */
void OLED_ShowImage(uint8_t X, uint8_t Page, uint8_t Width, uint8_t Height, const uint8_t *Image);

/**
 * @brief 在指定位置为原点绘制图形
 *
 * @param X 列(由于一个字符8*8,所以列数为128/8=16列,范围0~15)
 * @param Page 页(共8行,范围0~7)
 * @param ChineseWord 要打印的汉字字面量
 */
void OLED_ShowChinese(uint8_t X, uint8_t Page, uint8_t *ChineseWord);

/**
 * @brief 更新缓冲区数据到GDDRAM
 *
 */
void OLED_Update(void);

/**
 * @brief 使用I2C的DMA直接传输数据
 *
 */
void OLED_WriteData_DMA(void);

/**
 * @brief 使用DMA更新屏幕信息
 *
 */
void OLED_Update_DMA(void);

#endif // !__OLED_H