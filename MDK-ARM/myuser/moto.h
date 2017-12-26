#ifndef __MOTO_H
#define __MOTO_H

#define lunzi 65*3.14159 //mm
#include "stm32f4xx_hal.h"
void moto_90(unsigned char SF);
void moto_front(void);
void moto_back(void);
void moto_left(unsigned char a);
void moto_right(unsigned char a);
void moto_stop(void);
void moto_frontDis(int16_t setDisA,int16_t setDisB,int32_t speed1);
void moto_backDis(int16_t setDisA,int16_t setDisB,int32_t speed1);
void moto_jztask(void const * argument);
void bztask(void const * argument);
void moto_controltask(void const * argument);
int16_t moto_angle(unsigned char SF,int16_t setangle,unsigned char a);

#endif
