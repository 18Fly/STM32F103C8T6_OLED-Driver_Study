#include "OLED_Buf.h"

uint8_t OLED_Buf[8][128] = {0};

void OLED_Init(void)
{
    // 1. 使能 GPIOB 和 I2C1 时钟
    // ++ 配置DMA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 2. 配置 PB6 (SCL) 和 PB7 (SDA) 为复用开漏输出
    // ++ 配置DMA
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // 复用开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel6);

    // 3. 配置 I2C1
    // ++ 配置DMA
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1         = 0x00; // 主机模式可随意
    I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed          = 100000; // 100kHz
    I2C_Init(I2C1, &I2C_InitStructure);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&OLED_Buf;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize         = 128;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    // 4. 使能 I2C1
    I2C_Cmd(I2C1, ENABLE);
    // ++ 配置I2C的DMA 使能
    I2C_DMACmd(I2C1, ENABLE);

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

void OLED_WriteData(uint8_t *Data)
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
    for (uint8_t i = 0; i < 128; i++) {
        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
        I2C_SendData(I2C1, Data[i]); // Data Byte 数据
    }
    WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void OLED_WriteData_DMA(void)
{
    for (uint8_t page = 0; page < 8; page++) {
        OLED_SetPoint(0, page); // 设置页地址

        I2C_GenerateSTART(I2C1, ENABLE);
        WAIT_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
        I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);
        WAIT_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
        I2C_SendData(I2C1, 0x40); // 控制字节
        WAIT_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);

        DMA1_Channel6->CMAR  = (uint32_t)&OLED_Buf[page];
        DMA1_Channel6->CNDTR = 128;

        DMA_Cmd(DMA1_Channel6, ENABLE);
        while (!DMA_GetFlagStatus(DMA1_FLAG_TC6));
        DMA_Cmd(DMA1_Channel6, DISABLE);

        // ★★ 等待I2C数据全部发送完成 ★★ tips:不等待的话每页最后一列附近会产生随机小乱码
        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET);

        I2C_GenerateSTOP(I2C1, ENABLE);
        DMA_ClearFlag(DMA1_FLAG_TC6);
    }
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
        for (uint8_t j = 0; j < 128; j++)
            OLED_Buf[i][j] = 0x00;
    }
}

void OLED_ShowChar(uint8_t X, uint8_t Page, uint8_t Char)
{
    // OLED_SetPoint(X * 8, Page);
    for (uint8_t i = 0; i < 8; i++) {
        // OLED_WriteData(OLED_Font8x8[Char - 0x20][i]);
        OLED_Buf[Page][X * 8 + i] = OLED_Font8x8[Char - 0x20][i];
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
        // OLED_SetPoint(X, Page + i);
        for (uint8_t j = 0; j < Width; j++) {
            // OLED_WriteData(Image[i * Width + j]);
            OLED_Buf[Page + i][X + j] = Image[i * Width + j];
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
            if (attackIndex != -1) break;
        }

        OLED_ShowImage(X + i * 16, Page, 16, 2, Chinese[attackIndex].Chinese_Word);
        ChineseWord += 3;
        i++;
    }
}

void OLED_Update(void)
{
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetPoint(0, i);
        OLED_WriteData(OLED_Buf[i]);
    }
}

void OLED_Update_DMA(void)
{
    OLED_WriteData_DMA();
}
