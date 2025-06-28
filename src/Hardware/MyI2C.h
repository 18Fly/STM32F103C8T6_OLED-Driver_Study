#ifndef __MyI2C_H
#define __MyI2C_H

#include "stm32f10x.h"
#include "Delay.h"

#define PORT    GPIOB
#define SCL_Pin GPIO_Pin_10
#define SDA_Pin GPIO_Pin_11

/**
 * @brief I2C通讯协议初始化(软件模拟)
 *
 */
void MyI2C_Init(void);

/**
 * @brief 通讯协议开始通信
 *
 */
void MyI2C_Start(void);

/**
 * @brief 通讯协议停止通信
 *
 */
void MyI2C_Stop(void);

/**
 * @brief 发送1字节信息
 *
 * @param Bit 要发的1字节信息
 */
void MyI2C_SendByte(uint8_t Bit);

/**
 * @brief 读取1字节信息
 *
 * @return uint8_t 响应到的信息
 */
uint8_t MyI2C_ReadByte(void);

/**
 * @brief 发送应答
 *
 * @param Ack 应答信息
 */
void MyI2C_SendAck(uint8_t Ack);

/**
 * @brief 响应应答
 *
 * @return uint8_t 响应的信息
 */
uint8_t MyI2C_ReadAck(void);

#endif // !__MyI2C_H
