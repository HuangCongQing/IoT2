#include <stdio.h>
#include<ioCC2530.h> //引入CC2530对应的头文件
#include"sys_init.h"
#include"uart.h"
int getADC(void) ;
__near_func int putchar(int ch)
{
    U0DBUF = ch;
    while(UTX0IF == 0);
    UTX0IF = 0;
    return ch;
}

void Photoresistance_Test(void)
{
  char StrAdc[10];
  int AdcValue; 
  
  AdcValue = getADC();
  sprintf(StrAdc,"%d\r\n",AdcValue);
  printf("%s",StrAdc);                //串口发送数据
  delay(10000);                      //延时 
}

/*得到ADC值
-------------------------------------------------------*/
int getADC(void) 
{
  unsigned int  value;
  
  P0SEL |= 0x01;
  ADCCON3  = (0xB0);                    //选择AVDD5为参考电压；12分辨率；P0_0  ADC
  
  ADCCON1 |= 0x30;                      //选择ADC的启动模式为手动
  ADCCON1 |= 0x40;                      //启动AD转化             
  
  while(!(ADCCON1 & 0x80));             //等待ADC转化结束
  
  value =  ADCL >> 2;
  value |= (ADCH << 6);                 //取得最终转化结果，存入value中
  
  return ((value) >> 2);        
}



void main(void)
{
  xtal_init();             //系统时钟初始化       
  uart0_init(0x00,0x00);  //初始化串口：无奇偶校验，停止位为1位      
  while(1)
  {           
   Photoresistance_Test();
  }
}    
    