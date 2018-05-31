/*
 * rav4_vfd.h
 *
 *  Created on: 2018Äê5ÔÂ31ÈÕ
 *      Author: 10623
 */

#ifndef RAV4_VFD_H_
#define RAV4_VFD_H_

#include "main.h"
#include "stm32f1xx_hal.h"


typedef unsigned char 	u8;
typedef unsigned short 	u16;

void writebyte(unsigned char temp);
void display_graphic_32x32(u8 page,u8 column,u8 *dp);
void display_graphic_16x32(u8 page,u8 column,u8 *dp);
void display_graphic_32x64(u8 page,u8 column,u8 *dp);
u8 byte_reverse(u8 src);
void display_string_5x8(u8 page,u8 column,unsigned char *dp);
void display_string_8x16(u8 page,u8 column,unsigned char *dp);
void Rav4_Vfd_Init();
void Rav4_Display();

#endif /* RAV4_VFD_H_ */
