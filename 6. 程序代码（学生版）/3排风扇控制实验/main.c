#include<ioCC2530.h> //����CC2530��Ӧ��ͷ�ļ�
#include<string.h>   //�����ַ�������������
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*���ȳ�ʼ��*/
void Fan_init(void)
{
  P0SEL &=~0x02;  //����P0_1Ϊ��ͨIO
  P0DIR |= 0x02; //����P0_1Ϊ ���
  P0_1  =  0;   //����Ĭ�Ϲر�
}

/*���ڽ����뷢�ͺ���*/
void uart_test(void)
{
  ch = Uart_Recv_char(); //�����ڽ����뷢�ͺ�������һ��,���ڵ��ã�
                        //  ��ʵ��Ϊ�����������ڲ�����
  Uart_Send_char(ch);
}


void main(void)
{
  Fan_init(); //����Ӳ����ʼ��
  xtal_init();//ϵͳʱ�ӳ�ʼ��       
  uart0_init(0x00, 0x00);            //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ
  Uart_Send_String("���ڿ��Ʒ���ʵ��\r\n");
  P0_6 = 0;                  //����Ϊ����ģʽ(485���ƶ�)
  while(1)
  {
   ch = Uart_Recv_char(); //���ڽ����ֽں���
    if (ch == 1)
    {
      	P0_1= 1;           //���ڽ��յ���01�����ŷ���
    }
    if(ch == 0)       
    {                   //���ڽ��յ���00���ر��ŷ���
      P0_1 = 0;
    }
    P0_6 = 1;         //485���ƶ�����Ϊ�ߵ�ƽ��485��������Ϊ����ģʽ
    Uart_Send_char(ch);
    P0_6 = 0;        //485���ƶ�����Ϊ�͵�ƽ����������Ϊ����ģʽ���ٴν��մ�������

   }
}
    
    