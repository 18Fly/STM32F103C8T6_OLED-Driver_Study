#include "IIC.h"

void IIC_Init(void)
{
    // 1. 使能 GPIOB 和 I2C1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 2. 配置 PB6 (SCL) 和 PB7 (SDA) 为复用开漏输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // 复用开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 配置 I2C1
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1         = 0x00; // 主机模式可随意
    I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed          = 100000; // 100kHz
    I2C_Init(I2C1, &I2C_InitStructure);

    // 4. 使能 I2C1
    I2C_Cmd(I2C1, ENABLE);
}

void IIC_Start(void)
{
    I2C_GenerateSTART(I2C1, ENABLE);
}

void IIC_Stop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void IIC_SendData(uint8_t Data)
{
    I2C_SendData(I2C1, Data);
}

uint8_t IIC_ReceiveData(void)
{
    return I2C_ReceiveData(I2C1);
}
