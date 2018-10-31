#include <ioCC2530.h>             //引入CC2530所对应的头文件（包含各SFR的定义）
#include "sys_init.h"

/*系统时钟初始化
-------------------------------------------------------*/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;              //都上电
  while(!(CLKCONSTA & 0x40));     //晶体振荡器开启且稳定
  CLKCONCMD &= ~0x47;             //选择32MHz晶体振荡器
  SLEEPCMD |= 0x04;
}

/*uart0初始化
-------------------------------------------------------*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //初始化UART0端口P0_2，P0_3
  PERCFG&= ~0x01;                 //选择UART0为可选位置一
  U0CSR = 0xC0;                   //设置为UART模式,而且使能接受器
   
  U0GCR  = 0x09;
  U0BAUD = 0x3b;                  //设置UART0波特率为19200bps  
  
  U0UCR |= StopBits|Parity;       //设置停止位与奇偶校验
  
  P0SEL &= ~0x40;                //P0_6为普通 I/O 口(485控制端)
  P0DIR |= 0x40;                //P0_6为输出端口(485控制端)
 
  P0_6 = 1;
}


