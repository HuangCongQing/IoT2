#include <ioCC2530.h>             //����CC2530����Ӧ��ͷ�ļ���������SFR�Ķ��壩
#include "sys_init.h"

/*ϵͳʱ�ӳ�ʼ��
-------------------------------------------------------*/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;              //���ϵ�
  while(!(CLKCONSTA & 0x40));     //���������������ȶ�
  CLKCONCMD &= ~0x47;             //ѡ��32MHz��������
  SLEEPCMD |= 0x04;
}

/*uart0��ʼ��
-------------------------------------------------------*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //��ʼ��UART0�˿�P0_2��P0_3
  PERCFG&= ~0x01;                 //ѡ��UART0Ϊ��ѡλ��һ
  U0CSR = 0xC0;                   //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U0GCR  = 0x09;
  U0BAUD = 0x3b;                  //����UART0������Ϊ19200bps  
  
  U0UCR |= StopBits|Parity;       //����ֹͣλ����żУ��
  
  P0SEL &= ~0x40;                //P0_6Ϊ��ͨ I/O ��(485���ƶ�)
  P0DIR |= 0x40;                //P0_6Ϊ����˿�(485���ƶ�)
 
  P0_6 = 1;
}


