#include <ioCC2530.h>             //����CC2530����Ӧ��ͷ�ļ���������SFR�Ķ��壩
#include "sys_init.h"

/*ϵͳʱ�ӳ�ʼ��*/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;              //���ϵ�
  while(!(CLKCONSTA & 0x40));     //���������������ȶ�
  CLKCONCMD &= ~0x47;             //ѡ��32MHz��������
  SLEEPCMD |= 0x04;
}

/*UART0��ʼ��*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //��ʼ��UART0�˿�
  PERCFG&= ~0x01;                 //ѡ��UART0Ϊ��ѡλ��һ
  P2DIR &= ~0xC0;                 //P0������Ϊ����0
  U0CSR = 0xC0;                   //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U0GCR = 0x09;
  U0BAUD = 0x3b;                  //����UART0������Ϊ19200bps  
  
  U0UCR |= StopBits|Parity;       //����ֹͣλ����żУ��
  
  P0SEL &= ~0x40;          //P1.0 P1.1Ϊ��ͨ I/O ��,485��������
  P0DIR |= 0x40;           //���
 
  P0_6 = 1;
}
/*UART1��ʼ��*/
void uart1_init(void)
{
  P1SEL |=  0xC0;                 //��ʼ��UART1�˿�,P1_6��P1_7��Ϊ����
  PERCFG|= 0x02;                 //ѡ��UART1Ϊ��ѡλ�ö�
  U1CSR = 0xC0;                   //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U1GCR = 0x09;
  U1BAUD = 0x3b;                  //����UART0������Ϊ19200bps  
  
  U1UCR |= 0x00;       //����ֹͣλ����żУ��
}

/*��ʱ����*/
void delay(unsigned int i)
{
  unsigned int j ,k;
  for(j=i;j>0;j--)
    for(k=110;k>0;k--);	
}
