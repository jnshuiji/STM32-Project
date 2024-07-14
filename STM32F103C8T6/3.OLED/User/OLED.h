#ifndef OLED_H
#define OLED_H

#include "I2C.h"

#define OLED_ADDRESS 0x78
#define OLED_CMD 0x00
#define OLED_DAT 0x40

void OLED_Write_CMD(u8 cmd);
void OLED_Write_DAT(u8 dat);
void OLED_Init();
void OLED_Fill(u8 fill_Data);
void OLED_SetPos(u8 x, u8 y);
void OLED_Show_String(u8 x, u8 y, u8 ch[], u8 TextSize);
void OLED_ShowCN(u8 x, u8 y, u8 N);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);
void OLED_ON();
void OLED_OFF();

#endif // OLED_H