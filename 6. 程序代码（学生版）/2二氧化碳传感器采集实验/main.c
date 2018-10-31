#include <stdio.h>
#include<ioCC2530.h> //����CC2530��Ӧ��ͷ�ļ�
#include"sys_init.h"
#include"uart.h"
int getADC(void) ;
__near_func int putchar(int ch)
{
    U0DBUF = ch;
    while(UTX0IF == 0);
    UTX0IF = 0;
    return ch;
}

void Photoresistance_Test(void)
{
  char StrAdc[10];
  int AdcValue; 
  
  AdcValue = getADC();
  sprintf(StrAdc,"%d\r\n",AdcValue);
  printf("%s",StrAdc);                //���ڷ�������
  delay(10000);                      //��ʱ 
}

/*�õ�ADCֵ
-------------------------------------------------------*/
int getADC(void) 
{
  unsigned int  value;
  
  P0SEL |= 0x01;
  ADCCON3  = (0xB0);                    //ѡ��AVDD5Ϊ�ο���ѹ��12�ֱ��ʣ�P0_0  ADC
  
  ADCCON1 |= 0x30;                      //ѡ��ADC������ģʽΪ�ֶ�
  ADCCON1 |= 0x40;                      //����ADת��             
  
  while(!(ADCCON1 & 0x80));             //�ȴ�ADCת������
  
  value =  ADCL >> 2;
  value |= (ADCH << 6);                 //ȡ������ת�����������value��
  
  return ((value) >> 2);        
}



void main(void)
{
  xtal_init();             //ϵͳʱ�ӳ�ʼ��       
  uart0_init(0x00,0x00);  //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ      
  while(1)
  {           
   Photoresistance_Test();
  }
}    
    