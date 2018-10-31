#include<ioCC2530.h> //引入CC2530对应的头文件
#include<string.h>   //包含字符串处理函数声明
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*水泵初始化*/
void Pump_init(void)
{
 
}

/*串口接收与发送函数*/
void uart_test(void)
{
  ch = Uart_Recv_char(); //将串口接收与发送函数放在一起,便于调用，
                        //  此实验为单独调用其内部函数
  Uart_Send_char(ch);
}


void main(void)
{
  Pump_init(); //水泵硬件初始化
  xtal_init();//系统时钟初始化       
  uart0_init(0x00, 0x00);            //初始化串口：无奇偶校验，停止位为1位
  Uart_Send_String("串口控制水泵实验\r\n");
  P0_6 = 0;                  //设置为接收模式(485控制端)
  while(1)
  {
    
   }
}
    
    