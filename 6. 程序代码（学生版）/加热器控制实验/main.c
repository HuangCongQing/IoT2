#include<ioCC2530.h> //引入CC2530对应的头文件
#include<string.h>   //包含字符串处理函数声明
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*加热器初始化*/
void Hottor_init(void)
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
  
}
    
    