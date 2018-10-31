/********************************************************************************
* Copyright (c) 2015，联创中控（北京）科技有限公司
* All rights reserved.
*
* 文件名称：AM2321b.h
* 摘	要：温室大棚区功能函数
*
* 当前版本：V1.0
* 作	者：张小刚
* 完成日期：2016年4月7日
* 修改摘要：
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