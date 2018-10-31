#include<ioCC2530.h> //引入CC2530对应的头文件
#include<string.h>   //包含字符串处理函数声明
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*加热器初始化*/
void Hottor_init(void)
{
  P0SEL &=~0x02;  //配置P0_1为普通IO
  P0DIR |= 0x02; //配置P0_1为 输出
  P0_1  =  0;   //加热器默认关闭
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
   Hottor_init(); //加热器硬件初始化
  xtal_init();//系统时钟初始化       
  uart0_init(0x00, 0x00);            //初始化串口：无奇偶校验，停止位为1位
  Uart_Send_String("串口控制加热器实验\r\n");
  P0_6 = 0;                  //设置为接收模式(485控制端)
  while(1)
  {
    ch = Uart_Recv_char(); //串口接收字节函数
    if (ch == 1)
    {
      P0_1= 1;           //串口接收到“01”打开加热器
    }
    if(ch == 0)       
    {                   //串口接收到“00”关闭加热器
      P0_1 = 0;
    }
    P0_6 = 1;         //设置为发送模式(485控制端)
    Uart_Send_char(ch);
    P0_6 = 0;        //设置为接收模式(485控制端)再次接收串口数据
   }
}
    
    