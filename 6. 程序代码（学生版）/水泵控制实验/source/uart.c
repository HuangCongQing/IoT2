#include <ioCC2530.h>             //����CC2530����Ӧ��ͷ�ļ���������SFR�Ķ��壩
#include "uart.h"

/*���ڷ����ֽں���
-------------------------------------------------------*/
void Uart_Send_char(char ch)
{
  U0DBUF = ch;
  while(UTX0IF == 0);
  UTX0IF = 0;
}

/*���ڷ����ַ�������
-------------------------------------------------------*/
void Uart_Send_String(char *Data)
{  
  while (*Data != '\0')
  {
    Uart_Send_char(*Data++);
  }
}



/*���ڽ����ֽں���
-------------------------------------------------------*/
int Uart_Recv_char(void)
{
  int ch;
    
  while (URX0IF == 0);
  ch = U0DBUF;
  URX0IF = 0;
  return ch;
}