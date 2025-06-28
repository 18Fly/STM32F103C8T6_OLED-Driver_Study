#include "OLED.h"

void OLED_Init(void)
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

    // 配置OLED通讯的基础设置
    // Power up VDD
    Delay_ms(100);

    OLED_WriteCommand(0xAE); // 关闭显示

    OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // 设置显示开始行

    OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6); // 设置正常/倒转显示

    OLED_WriteCommand(0x8D); // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // 开启显示

    // 手册建议增加100ms延迟
    Delay_ms(100);
}

void OLED_WriteCommand(uint8_t Command)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter); // 寻址
    WAIT_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, 0x00); // Control Byte 写命令
    WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    I2C_SendData(I2C1, Command); // Data Byte 命令
    WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void OLED_WriteData(uint8_t Data)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter); // 寻址
    WAIT_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, 0x40); // Control Byte 写数据
    WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    I2C_SendData(I2C1, Data); // Data Byte 数据
    WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void OLED_SetPoint(uint8_t X, uint8_t Page)
{
    OLED_WriteCommand(0x00 | X & 0x0F);
    OLED_WriteCommand(0x10 | (X & 0xF0) >> 4);
    OLED_WriteCommand(0xB0 | Page);
}

void OLED_ClearScreen(void)
{
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetPoint(0, i);
        for (uint8_t j = 0; j < 128; j++)
            OLED_WriteData(0x00);
    }
}

void OLED_ShowChar(uint8_t X, uint8_t Page, uint8_t Char)
{
    OLED_SetPoint(X * 8, Page);
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteData(OLED_Font8x8[Char - 0x20][i]);
    }
}

void OLED_ShowString(uint8_t X, uint8_t Page, char *String)
{
    while (*String) {
        OLED_ShowChar(X, Page, *String);
        X++;
        if (X > 15) {
            X = 0;
            Page++;
        }
        String++;
    }
}

void OLED_ShowImage(uint8_t X, uint8_t Page, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    for (uint8_t i = 0; i < Height; i++) {
        OLED_SetPoint(X, Page + i);
        for (uint8_t j = 0; j < Width; j++) {
            OLED_WriteData(Image[i * Width + j]);
        }
    }
}

void OLED_ShowChinese(uint8_t X, uint8_t Page, uint8_t *ChineseWord)
{
    char attackIndex = -1;
    uint8_t i        = 0;
    while (*ChineseWord) {
        for (uint8_t i = 0; i < sizeof(Chinese) / sizeof(Chinese[0]); i++) {
            attackIndex = i;
            for (uint8_t j = 0; j < 3; j++) {
                if (ChineseWord[j] != Chinese[i].Chinese_Index[j]) {
                    attackIndex = -1;
                    break;
                }
            }
            if (attackIndex != -1)
                break;
        }
        OLED_ShowImage(X + i * 16, Page, 16, 2, Chinese[attackIndex].Chinese_Word);
        ChineseWord += 3;
        i++;
    }
}
