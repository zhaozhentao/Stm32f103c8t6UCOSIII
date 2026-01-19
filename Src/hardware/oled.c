#include <os.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "uart.h"
#include "oled.h"

u32 fontAddr = 0;

#define OLED_DC_Clr()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define OLED_DC_Set()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define OLED_CS_Clr()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define OLED_CS_Set()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)

#define OLED_ROM_CS_Clr()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define OLED_ROM_CS_Set()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)

#define OLED_CMD  0    //写命令
#define OLED_DATA 1    //写数据

extern SPI_HandleTypeDef hspi1;
extern OS_MUTEX spiMutex;

// GND
// VCC
// clk       PA5
// MOSI      PA7
// DC        PA4
// CS1       PA1
// FSO/MISO  PA6 输入
// CS2       PA8

//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat, u8 cmd) {
    OS_ERR err;
    OS_TICK ts;

    OSMutexPend(&spiMutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);

    // 1. 控制 DC
    if (cmd)
        OLED_DC_Set();   // 写数据
    else
        OLED_DC_Clr();   // 写命令

    // 2. 拉低 CS ? 开始 SPI 会话
    OLED_CS_Clr();

    // 3. 使用硬件 SPI 写 1 字节
    HAL_SPI_Transmit(&hspi1, &dat, 1, 10);

    OLED_CS_Set();

    // 4. 拉高 CS ? 结束 SPI 会话
    OLED_DC_Set();

    OSMutexPost(&spiMutex, OS_OPT_POST_NONE, &err);
}

//清屏函数
void OLED_Clear() {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);//设置页地址
        OLED_WR_Byte(0x10, OLED_CMD);  //设置列地址的高4位
        OLED_WR_Byte(0x00, OLED_CMD);  //设置列地址的低4位
        for (n = 0; n < 128; n++) {
            OLED_WR_Byte(0x00, OLED_DATA);//清除所有数据
        }
    }
}

//设置起始地址
void OLED_address(u8 x, u8 y) {
    OLED_WR_Byte(0xb0 + y, OLED_CMD);              //设置页地址
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);  //设置列地址的高4位
    OLED_WR_Byte((x & 0x0f), OLED_CMD);            //设置列地址的低4位
}

// 未检查
void OLED_Init() {
    OS_ERR err;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    OSTimeDlyHMSM(0, 0, 0, 200, OS_OPT_TIME_DLY, &err);

    OLED_WR_Byte(0xAE, OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);//
    OLED_WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

//反显函数
void OLED_ColorTurn(u8 i) {
    if (i == 0) {
        OLED_WR_Byte(0xA6, OLED_CMD);//正常显示
    }
    if (i == 1) {
        OLED_WR_Byte(0xA7, OLED_CMD);//反色显示
    }
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i) {
    if (i == 0) {
        OLED_WR_Byte(0xC8, OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1) {
        OLED_WR_Byte(0xC0, OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
void OLED_Display_8x16(u8 x, u8 y, u8 *dp) {
    u8 i, j;
    for (j = 0; j < 2; j++) {
        OLED_address(x, y);
        for (i = 0; i < 8; i++) {
            OLED_WR_Byte(*dp, OLED_DATA);   //写数据到LCD,每写完一个8位的数据后列地址自动加1
            dp++;
        }
        y++;
    }
}

//送指令到晶联讯字库IC
void Send_Command_to_ROM(u8 dat) {
    HAL_SPI_Transmit(&hspi1, &dat, 1, 10);
}

//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
void OLED_Display_16x16(u8 x, u8 y, u8 *dp) {
    u8 i, j;
    for (j = 0; j < 2; j++) {
        OLED_address(x, y);
        for (i = 0; i < 16; i++) {
            OLED_WR_Byte(*dp, OLED_DATA);  //写数据到OLED,每写完一个8位的数据后列地址自动加1
            dp++;
        }
        y++;
    }
}

//从晶联讯字库IC中取汉字或字符数据（1个字节）
uint8_t Get_data_from_ROM(void) {
    uint8_t rx = 0;
    uint8_t dummy = 0xFF;  // 主机必须发送才能生成时钟
    HAL_SPI_TransmitReceive(&hspi1, &dummy, &rx, 1, 10);
    return rx;
}

//从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pbuff的地址
//连续读取
void OLED_get_data_from_ROM(u8 addrHigh, u8 addrMid, u8 addrLow, u8 *pbuff, u8 DataLen) {
    u8 i;
    OLED_ROM_CS_Clr();
    Send_Command_to_ROM(0x03);
    Send_Command_to_ROM(addrHigh);
    Send_Command_to_ROM(addrMid);
    Send_Command_to_ROM(addrLow);
    for (i = 0; i < DataLen; i++) {
        *(pbuff + i) = Get_data_from_ROM();
    }
    OLED_ROM_CS_Set();
}

void OLED_Display_GB2312_string(u8 x, u8 y, u8 *text) {
    u8 i = 0;
    u8 addrHigh, addrMid, addrLow;
    u8 fontbuf[32];
    while (text[i] > 0x00) {
        if ((text[i] >= 0xb0) && (text[i] <= 0xf7) && (text[i + 1] >= 0xa1)) {
            //国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
            //Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
            //由于担心8位单片机有乘法溢出问题，所以分三部取地址
            fontAddr = (text[i] - 0xb0) * 94;
            fontAddr += (text[i + 1] - 0xa1) + 846;
            fontAddr = fontAddr * 32;

            addrHigh = (fontAddr & 0xff0000) >> 16;   //地址的高8位,共24位
            addrMid = (fontAddr & 0xff00) >> 8;       //地址的中8位,共24位
            addrLow = (fontAddr & 0xff);            //地址的低8位,共24位

            OLED_get_data_from_ROM(addrHigh, addrMid, addrLow, fontbuf, 32);
            //取32个字节的数据，存到"fontbuf[32]"
            OLED_Display_16x16(x, y, fontbuf);
            //显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据
            x += 16;
            i += 2;
        } else if ((text[i] >= 0xa1) && (text[i] <= 0xa3) && (text[i + 1] >= 0xa1)) {

            fontAddr = (text[i] - 0xa1) * 94;
            fontAddr += (text[i + 1] - 0xa1);
            fontAddr = fontAddr * 32;

            addrHigh = (fontAddr & 0xff0000) >> 16;
            addrMid = (fontAddr & 0xff00) >> 8;
            addrLow = (fontAddr & 0xff);

            OLED_get_data_from_ROM(addrHigh, addrMid, addrLow, fontbuf, 32);
            OLED_Display_16x16(x, y, fontbuf);
            x += 16;
            i += 2;
        } else if ((text[i] >= 0x20) && (text[i] <= 0x7e)) {
            unsigned char fontbuf[16];
            fontAddr = (text[i] - 0x20);
            fontAddr = (unsigned long) (fontAddr * 16);
            fontAddr = (unsigned long) (fontAddr + 0x3cf80);

            addrHigh = (fontAddr & 0xff0000) >> 16;
            addrMid = (fontAddr & 0xff00) >> 8;
            addrLow = fontAddr & 0xff;

            OLED_get_data_from_ROM(addrHigh, addrMid, addrLow, fontbuf, 16);
            OLED_Display_8x16(x, y, fontbuf);
            x += 8;
            i += 1;
        } else
            i++;
    }
}
