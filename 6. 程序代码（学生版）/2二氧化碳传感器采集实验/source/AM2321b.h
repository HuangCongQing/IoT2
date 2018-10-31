/********************************************************************************
* Copyright (c) 2015�������пأ��������Ƽ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�AM2321b.h
* ժ	Ҫ�����Ҵ��������ܺ���
*
* ��ǰ�汾��V1.0
* ��	�ߣ���С��
* ������ڣ�2016��4��7��
* �޸�ժҪ��
********************************************************************************/


#ifndef AM232XX_H
#define AM232XX_H

#include <ioCC2530.h>

typedef signed   char   int8;
typedef unsigned char   uint8;

typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;


void AM232XX_Init(void);
void Get_Temp(uint8 *temp,uint8 *humi);


#endif