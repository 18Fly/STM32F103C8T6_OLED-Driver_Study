#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/**
 * @brief PC13上的LED初始化
 *
 */
void LED_Init(void);

/**
 * @brief PC13上的LED控制开启
 *
 */
void LED_On(void);

/**
 * @brief PC13上的LED控制关闭
 *
 */
void LED_Off(void);

#endif // !__LED_H
