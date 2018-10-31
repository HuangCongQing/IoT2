#include <ioCC2530.h>             //引入CC2530所对应的头文件（包含各SFR的定义）
#include "sys_init.h"

/*系统时钟初始化*/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;              //都上电
  while(!(CLKCONSTA & 0x40));     //晶体振荡器开启且稳定
  CLKCONCMD &= ~0x47;             //选择32MHz晶体振荡器
  SLEEPCMD |= 0x04;
}

/*UART0初始化*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //初始化UART0端口
  PERCFG&= ~0x01;                 //选择UART0为可选位置一
  P2DIR &= ~0xC0;                 //P0优先作为串口0
  U0CSR = 0xC0;                   //设置为UART模式,而且使能接受器
   
  U0GCR = 0x09;
  U0BAUD = 0x3b;                  //设置UART0波特率为19200bps  
  
  U0UCR |= StopBits|Parity;       //设置停止位与奇偶校验
  
  P0SEL &= ~0x40;          //P1.0 P1.1为普通 I/O 口,485控制引脚
  P0DIR |= 0x40;           //输出
 
  P0_6 = 1;
}
/*UART1初始化*/
void uart1_init(void)
{
  P1SEL |=  0xC0;                 //初始化UART1端口,P1_6，P1_7作为串口
  PERCFG|= 0x02;                 //选择UART1为可选位置二
  U1CSR = 0xC0;                   //设置为UART模式,而且使能接受器
   
  U1GCR = 0x09;
  U1BAUD = 0x3b;                  //设置UART0波特率为19200bps  
  
  U1UCR |= 0x00;       //设置停止位与奇偶校验
}

/*延时函数*/
void delay(unsigned int i)
{
  unsigned int j ,k;
  for(j=i;j>0;j--)
    for(k=110;k>0;k--);	
}
