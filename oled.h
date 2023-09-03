#ifndef __OLED_H_
#define __OLED_H_	

#include "stdint.h"
#define PAGE_SIZE    	8
#define XLevelL				0x00
#define XLevelH				0x10
#define YLevel       	0xB0
#define OLED_WIDTH 	     	128
#define OLED_HEIGHT 	     	64


	
void WriteCmd(void);
//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd);
//向设备写数据
void OLED_WR_DATA(uint8_t data);
//初始化oled屏幕
void OLED_Init(void);
	//清屏
void OLED_Clear(void);
//清行
void OLED_Clearrow(uint8_t i);
//开启OLED显示    
void OLED_Display_On(void);
//关闭OLED显示     
void OLED_Display_Off(void);
//设置光标
void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_On(void);
	
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);

 //显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size_option);
void OLED_ShowNum_CustomSize(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2);

//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);

//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

//显示图片
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP1[]);
//清除多行
void OLED_ClearRows(uint8_t startRow, uint8_t endRow);
//清除指定的行列之间的内容，前两个参数为行，后两个为列
void OLED_ClearRegion(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol);
//在原来的基础上添加了一个size控制大小
void OLED_ShowString_Sized(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t text_size);
//显示字符串的宽度大小
void OLED_ShowString_Compressed(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t spacing);
void OLED_DrawBMP_Sized(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[], uint8_t size);
//画点函数
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
//画线函数
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_ClearRegion(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol);
void OLED_ClearColumns(uint8_t startCol, uint8_t endCol);
void OLED_ClearColumn(uint8_t col);
void OLED_ClearRows(uint8_t startRow, uint8_t endRow);
void OLED_Clearrow(uint8_t i);
#endif
