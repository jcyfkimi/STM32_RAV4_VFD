/*
 * rav4_vfd.c
 *
 *  Created on: 2018年5月31日
 *      Author: 10623
 */

#include "main.h"
#include "rav4_vfd.h"
#include "code_table.h"

u8 Display_ram[255][8];

void writebyte(unsigned char temp)//写数据
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, VFDSCK_Pin, GPIO_PIN_RESET);
		if (temp & 0x80)
		{
			HAL_GPIO_WritePin(GPIOB, VFDDA_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, VFDDA_Pin, GPIO_PIN_RESET);
		}
		temp <<= 1;
		HAL_GPIO_WritePin(GPIOB, VFDSCK_Pin, GPIO_PIN_SET);
	}
}

//显示 32x32 点阵图像、汉字、生僻字或 32x32 点阵的其他图标
void display_graphic_32x32(u8 page,u8 column,u8 *dp)
{
	u8 X,Y;
	for(Y=0;Y<4;Y++)
	{
		for(X=0;X<32;X++)
		{
			Display_ram[X+column][Y+page]=*dp;
			dp++;
		}
	}
}
//显示 16x32 点阵图像、汉字、生僻字或 32x32 点阵的其他图标
void display_graphic_16x32(u8 page,u8 column,u8 *dp)
{
	u8 X,Y;

	for(Y=0;Y<4;Y++)
	{
		for(X=0;X<16;X++)
		{
			Display_ram[X+column][Y+page]=*dp;
			dp++;
		}
	}
}

//显示 32x64 点阵图像、汉字、生僻字或 32x64 点阵的其他图标
void display_graphic_32x64(u8 page,u8 column,u8 *dp)
{
	u8 X,Y;

	for(Y=0;Y<8;Y++)
	{
		for(X=0;X<32;X++)
		{
			Display_ram[X+column][Y+page]=*dp;
			dp++;
		}
	}
}

u8 byte_reverse(u8 src)//字节倒序
{
    u8 des = 0;
    int i=0;
    for(;i<8;i++)
    {
        if(src & 0x1)
        des |= 1 << (7-i);
        else
        des &= ~(1 << (7-i));
        src >>= 1;
    }
    return des;
}
void display_string_5x8(u8 page,u8 column,unsigned char *dp)
{
	u16 i = 0, j, k;
	while (dp[i] > 0x00)
	{
		if ((dp[i] >= 0x20) && (dp[i] <= 0x7e))
		{
			j = dp[i] - 0x20;
			for (k = 0; k < 5; k++)
			{
				Display_ram[k + column][page] = byte_reverse(ascii_table_5x8[j][k]);
			}
			i++;
			column += 6;
		}
		else
		{
			i++;
		}
	}
}

void display_string_8x16(u8 page,u8 column,unsigned char *dp)
{
	u16 i = 0, j, k, n;
	while (dp[i] > 0x00)
	{
		if ((dp[i] >= 0x20) && (dp[i] <= 0x7e))
		{
			j = dp[i] - 0x20;
			for (n = 0; n < 2; n++)
			{
				for (k = 0; k < 8; k++)
				{
					Display_ram[k + column][n + page] = byte_reverse(ascii_table_8x16[j][k + 8 * n]);
				}
			}
			i++;
			column += 8;
		}
		else
		{
			i++;
		}
	}
}

void Rav4_Display()
{
	unsigned char x=0,y=0;
	display_graphic_32x32(0,0,(u8 *)test_d[2]);
	display_graphic_32x32(0,32,(u8 *)test_d[1]);
	display_graphic_32x32(0,64,(u8 *)test_d[0]);


	display_string_8x16(4,1,(u8 *)"0123456789 ~!@#$%^&*()_+{}:;|");
	display_string_5x8(6,1,(u8 *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//	{
//		display_graphic_32x64(0,10+32*0,(u8 *)Number_32x64[(i/10000)%10]);
//		display_graphic_32x64(0,10+32*1,(u8 *)Number_32x64[(i/1000)%10]);
//		display_graphic_32x64(0,10+32*2,(u8 *)Number_32x64[(i/100)%10]);
//		display_graphic_32x64(0,10+32*3,(u8 *)Number_32x64[(i/10)%10]);
//		display_graphic_32x64(0,10+32*4,(u8 *)Number_32x64[i%10]);
//		i++;
//	}
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x0f);
	writebyte(0x00);
	writebyte(0xff);
	writebyte(0xfc);//开始
	for(x=0;x<255;x++)
	{
		for(y=0;y<8;y++)
		{
			writebyte(Display_ram[x][y]);
		}
	}
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);		//结束
	writebyte(0x01);
	writebyte(0x02);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
}


void Rav4_Vfd_Init()
{
    HAL_GPIO_WritePin(GPIOB, VFDSLEEP_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, VFDRST_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	writebyte(0x33);
	writebyte(0x40);
	writebyte(0x00 );
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x07);
	writebyte(0x3f);
	writebyte(0x7c);
	writebyte(0x00);
	writebyte(0x04);
	writebyte(0x01);
	writebyte(0x01);
	writebyte(0x01);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x8d);
	writebyte(0x04);
	writebyte(0xfc);
	writebyte(0x00);
	writebyte(0x80);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x05);
	writebyte(0x00);
	writebyte(0x00);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x03);
	writebyte(0x00);
	writebyte(0x00);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x0b);
	writebyte(0x00);
	writebyte(0xff);
	writebyte(0x02);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
	writebyte(0x05);
	writebyte(0x30);
	writebyte(0xff);  				  //与亮度有关
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, VFDCS_Pin, GPIO_PIN_RESET);
}



