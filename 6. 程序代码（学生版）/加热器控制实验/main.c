#include<ioCC2530.h> //����CC2530��Ӧ��ͷ�ļ�
#include<string.h>   //�����ַ�������������
#include"sys_init.h"
#include"uart.h"
void uart_test(void);
uchar ch;

/*��������ʼ��*/
void Hottor_init(void)
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
  
}
    
    