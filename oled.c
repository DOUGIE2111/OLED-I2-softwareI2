#include "stm32f1xx_hal.h"
#include "oledfont.h"
#include "oled.h"
#include <stdlib.h>
/*引脚配置*/
#define OLED_W_SCL(x)		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)(x))
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)(x))

unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
/*引脚初始化*/
void OLED_I2C_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WR_CMD(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WR_DATA(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_Set_Pos(uint8_t X, uint8_t Y)
{
	OLED_WR_CMD(0xB0 + Y);					//设置Y位置
	OLED_WR_CMD(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WR_CMD((X & 0x0F));			//设置X位置高4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */

void OLED_Clear()
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	} 
}

void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_DATA(1); 
	} //更新显示
}
/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8x16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8x16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_DATA(F6x8[c][i]);
				
			}
}
/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size_option)
{         	
    uint8_t t, temp;
    uint8_t enshow = 0;						   
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size_option / 2) * t, y, ' ', size_option);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }
        OLED_ShowChar(x + size_option * t, y, temp + '0', size_option * 2); // 调整 size_option 的值来放大显示
    }
}
/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
//  */
void OLED_ShowNum_CustomSize(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }

        OLED_ShowChar(x + (size2 * t), y, temp + '0', size2); // 控制显示大小
    }
}

void OLED_ShowString_Compressed(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t spacing)
{
    unsigned char j = 0;
    uint8_t charWidth = 0;
    
    if (Char_Size == 16) {
        charWidth = 8;
    } else if (Char_Size == 32) {
        charWidth = 16;
    } else {
        // 默认使用 Char_Size == 8 的字符宽度
        charWidth = 6;
    }
    
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += charWidth + spacing; // 增加字符宽度和间距
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}
void OLED_ShowString_Sized(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t text_size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size * text_size); // 调整字符的大小
        x += 8 * text_size; // 调整 x 坐标
        if (x > 120) 
        {
            x = 0;
            y += 2 * text_size; // 调整 y 坐标
        }
        j++;
    }
}
/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
//void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
//{
//	uint8_t i;
//	for (i = 0; i < Length; i++)							
//	{
//		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
//	}
//}

void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_DATA(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_DATA(Hzk[2*no+1][t]);
				adder+=1;
      }					
}

//显示图片
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_DATA(BMP[j++]);	  
	    }
	}
} 
void OLED_DrawBMP_Sized(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[], uint8_t size) {
    uint8_t j = 0;
    uint8_t x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;

    for (y = y0; y < y1; y++) {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++) {
            if (size == 8) {
                OLED_WR_DATA(BMP[j++]);
            } else if (size == 4) {
                uint8_t data = BMP[j++];
                uint8_t upper = (data >> 4) & 0x0F;
                uint8_t lower = data & 0x0F;
                OLED_WR_DATA((upper << 4) | upper);
                OLED_WR_DATA((lower << 4) | lower);
            }
        }
    }
}
// 绘制像素点
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= 0 && x < OLED_WIDTH && y >= 0 && y < OLED_HEIGHT) {
        uint8_t page = y / 8;
        uint8_t bit = y % 8;
        
        OLED_Set_Pos(x, page);
        
        if (color) {
            OLED_WR_DATA(1 << bit); // 设置像素点为白色
        } else {
            OLED_WR_DATA(~(1 << bit)); // 设置像素点为黑色
        }
    }
}
// 在指定的位置绘制一条直线
// x0, y0: 起始点坐标
// x1, y1: 终止点坐标
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        OLED_DrawPixel(x0, y0, 1); // 绘制像素点

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;
        
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

//清行
void OLED_Clearrow(uint8_t i)
{
	uint8_t n;
	OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
}
//清除多行
void OLED_ClearRows(uint8_t startRow, uint8_t endRow)
{
    if (startRow >= 8 || endRow >= 8 || startRow > endRow) {
        return; // 超出屏幕范围或参数错误，不处理
    }
    
    uint8_t row, col;
    
    for (row = startRow; row <= endRow; row++) {
        for (col = 0; col < 128; col++) {
            OLED_Set_Pos(col, row); // 设置显示位置为指定行和列
            OLED_WR_DATA(0);        // 清除该位置的内容
        }
    }
}

//清除单列
void OLED_ClearColumn(uint8_t col)
{
    uint8_t i;
    
    if (col >= 128) {
        return; // 超出屏幕范围，不处理
    }
    
    for (i = 0; i < 8; i++) {
        OLED_Set_Pos(col, i); // 设置显示位置为指定列和行
        OLED_WR_DATA(0);      // 清除该列对应的内容
    }
}
//清除多列
void OLED_ClearColumns(uint8_t startCol, uint8_t endCol)
{
    if (startCol >= 128 || endCol >= 128 || startCol > endCol) {
        return; // 超出屏幕范围或参数错误，不处理
    }
    
    uint8_t col;
    
    for (col = startCol; col <= endCol; col++) {
        OLED_ClearColumn(col);
    }
}
//清除指定的行列之间的内容，前两个参数为行，后两个为列
void OLED_ClearRegion(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol)
{
    if (startRow >= 8 || endRow >= 8 || startCol >= 128 || endCol >= 128 || startRow > endRow || startCol > endCol) {
        return; // 超出屏幕范围或参数错误，不处理
    }
    
    uint8_t row, col;
    
    for (row = startRow; row <= endRow; row++) {
        for (col = startCol; col <= endCol; col++) {
            OLED_Set_Pos(col, row); // 设置显示位置为指定行和列
            OLED_WR_DATA(0);        // 清除该位置的内容
        }
    }
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WR_CMD(0xAE);	//关闭显示
	
	OLED_WR_CMD(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WR_CMD(0x80);
	
	OLED_WR_CMD(0xA8);	//设置多路复用率
	OLED_WR_CMD(0x3F);
	
	OLED_WR_CMD(0xD3);	//设置显示偏移
	OLED_WR_CMD(0x00);
	
	OLED_WR_CMD(0x40);	//设置显示开始行
	
	OLED_WR_CMD(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WR_CMD(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WR_CMD(0xDA);	//设置COM引脚硬件配置
	OLED_WR_CMD(0x12);
	
	OLED_WR_CMD(0x81);	//设置对比度控制
	OLED_WR_CMD(0xCF);

	OLED_WR_CMD(0xD9);	//设置预充电周期
	OLED_WR_CMD(0xF1);

	OLED_WR_CMD(0xDB);	//设置VCOMH取消选择级别
	OLED_WR_CMD(0x30);

	OLED_WR_CMD(0xA4);	//设置整个显示打开/关闭

	OLED_WR_CMD(0xA6);	//设置正常/倒转显示

	OLED_WR_CMD(0x8D);	//设置充电泵
	OLED_WR_CMD(0x14);

	OLED_WR_CMD(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
