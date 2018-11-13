# IoT2
物联网综合实验二（2018年秋）

**PLUS**
* 波特率设置“19200”；
* `delay_ms(10000);`延迟1万ms,就是延迟10s
### 实验

* 加热器控制实验

* 二氧化碳传感器采集实验

* 排风扇控制实验

* 水泵控制实验

```
#include<ioCC2530.h> //引入CC2530对应的头文件
#include<string.h>   //包含字符串处理函数声明
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*水泵初始化*/
void Pump_init(void)
{
  P0SEL &=~0x02;  //配置P0_1为普通IO
  P0DIR |= 0x02; //配置P0_1为 输出
  P0_1  =  0;   //水泵默认关闭
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
     ch = Uart_Recv_char(); //串口接收字节函数
    if (ch == 1)
    {
     	 P0_1= 1;           //串口接收到“01”打开水泵
    }
    if(ch == 0)       
    {                   //串口接收到“00”关闭水泵
    	  P0_1 = 0;
    }
    P0_6 = 1;         //485控制端设置为高电平，485串口设置为发送模式
    Uart_Send_char(ch);
    P0_6 = 0;        //485控制端设置为低电平，串口设置为接收模式，再次接收串口数据

   }
}
    
    
```

### 软件

### 硬件



### License

Copyright (c) [ChungKing](https://github.com/HuangCongQing/). All rights reserved.

Licensed under the [MIT](./LICENSE) License.