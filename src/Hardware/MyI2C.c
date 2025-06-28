#include "MyI2C.h"

#define REG_WRITE_One(bit)  GPIO_SetBits(PORT, (bit))
#define REG_WRITE_Zero(bit) GPIO_ResetBits(PORT, (bit))
#define REG_READ_SDA        GPIO_ReadInputDataBit(PORT, SDA_Pin)

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SCL_Pin | SDA_Pin;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void MyI2C_Start(void)
{
    REG_WRITE_One(SDA_Pin);
    Delay_us(10);

    REG_WRITE_One(SCL_Pin);
    Delay_us(10);

    REG_WRITE_Zero(SDA_Pin);
    Delay_us(10);

    REG_WRITE_Zero(SCL_Pin);
    Delay_us(10);
}

void MyI2C_Stop(void)
{
    REG_WRITE_Zero(SDA_Pin);
    Delay_us(10);

    REG_WRITE_One(SCL_Pin);
    Delay_us(10);

    REG_WRITE_One(SDA_Pin);
    Delay_us(10);
}

void MyI2C_SendByte(uint8_t Bit)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (Bit & (0x80 >> i)) {
            REG_WRITE_One(SDA_Pin);
        } else {
            REG_WRITE_Zero(SDA_Pin);
        }
        Delay_us(10);

        REG_WRITE_One(SCL_Pin);
        Delay_us(10);

        REG_WRITE_Zero(SCL_Pin);
        Delay_us(10);
    }
}

uint8_t MyI2C_ReadByte(void)
{
    uint8_t Byte = 0;
    REG_WRITE_One(SDA_Pin);
    Delay_us(10);

    for (uint8_t i = 0; i < 8; i++) {
        REG_WRITE_One(SCL_Pin);
        Delay_us(10);
        Byte |= REG_READ_SDA << (7 - i);
        REG_WRITE_Zero(SCL_Pin);
        Delay_us(10);
    }

    return Byte;
}

void MyI2C_SendAck(uint8_t Ack)
{
    if (Ack)
        REG_WRITE_One(SDA_Pin);
    else
        REG_WRITE_Zero(SDA_Pin);
    Delay_us(10);
    REG_WRITE_One(SCL_Pin);
    Delay_us(10);
    REG_WRITE_Zero(SCL_Pin);
    Delay_us(10);
}

uint8_t MyI2C_ReadAck(void)
{
    uint8_t AckBit;

    REG_WRITE_One(SDA_Pin);
    Delay_us(10);
    REG_WRITE_One(SCL_Pin);
    Delay_us(10);

    AckBit = REG_READ_SDA;
    Delay_us(10);

    REG_WRITE_Zero(SCL_Pin);
    Delay_us(10);

    return AckBit;
}
