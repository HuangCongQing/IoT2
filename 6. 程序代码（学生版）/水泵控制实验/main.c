#include<ioCC2530.h> //����CC2530��Ӧ��ͷ�ļ�
#include<string.h>   //�����ַ�������������
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*ˮ�ó�ʼ��*/
void Pump_init(void)
{
 
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
  Pump_init(); //ˮ��Ӳ����ʼ��
  xtal_init();//ϵͳʱ�ӳ�ʼ��       
  uart0_init(0x00, 0x00);            //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ
  Uart_Send_String("���ڿ���ˮ��ʵ��\r\n");
  P0_6 = 0;                  //����Ϊ����ģʽ(485���ƶ�)
  while(1)
  {
    
   }
}
    
    