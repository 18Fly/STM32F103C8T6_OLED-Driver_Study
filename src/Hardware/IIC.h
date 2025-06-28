#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"

/**
 * @brief IIC初始化 PB6-SCL PB7-SDA
 *
 */
void IIC_Init(void);

/**
 * @brief IIC 发送起始信号
 *
 */
void IIC_Start(void);

/**
 * @brief IIC 发送停止信号
 *
 */
void IIC_Stop(void);

/**
 * @brief IIC 发送一字节数据
 *
 */
void IIC_SendData(uint8_t);

/**
 * @brief IIC 接收一字节数据
 *
 * @return uint8_t 接收的一字节数据
 */
uint8_t IIC_ReceiveData(void);

#endif // !__IIC_H