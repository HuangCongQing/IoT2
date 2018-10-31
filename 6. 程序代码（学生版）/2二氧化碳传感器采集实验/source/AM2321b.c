/********************************************************************************
* Copyright (c) 2015�������пأ��������Ƽ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�AM2312b.c
* ժ	Ҫ�����Ҵ��������ܺ���
*
* ��ǰ�汾��V1.0
* ��	�ߣ���С��
* ������ڣ�2016��4��7��
* �޸�ժҪ��
********************************************************************************/

/*************ͷ�ļ�********************/
#include "AM2321b.h"

/**************AM2312b ������ʹ�ö˿ڶ���*******************/
#define SDA                    P1_6		//�¶ȴ������������ŵĶ���
#define SDAPxSEL               P1SEL
#define SDAPxDIR               P1DIR
#define SDAPORTBIT             0x40

#define SCL                     P1_7		//�¶ȴ�����ʱ�����ŵĶ���
#define SCLPxSEL                P1SEL
#define SCLPxDIR                P1DIR
#define SCLPORTBIT              0x80

#define Clr_IIC_SCL()   SCL=0
#define Set_IIC_SCL()   SCL=1

#define Set_IIC_SDA()   SDA =1
#define Clr_IIC_SDA()   SDA =0
#define READ_SDA()      SDA
#define WRITE_SDA(x)    SDA=x    


/*********************************************************/

#define IIC_Add 0xB8    //������ַ
unsigned char IIC_CMD[]={0x03,0x00,0x04}; //����ʪ�������CRCУ�飩
unsigned char Buffer[8] = {0x00};
unsigned char WR_Flag;


/*****************���غ�������************************/
void IIC_Config(void);
void IIC_SDA_OUT(void);
void IIC_SDA_IN(void );
void Delay1us(uint16 t);
void delay_ms(uint16 ms);
void AM232XX_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void Ack(void);
void NoAck(void);
char Test_Ack(void);
void SendData(unsigned char buffer);
unsigned char ReceiveData(void);
char WriteNByte(unsigned char sla,unsigned char *s,unsigned char n);
char ReadNByte(unsigned char Sal, unsigned char *p,unsigned char n);
unsigned int CRC16(unsigned char *ptr, unsigned char len);
unsigned char CheckCRC(unsigned char *ptr,unsigned char len);
void Waken(void);

/***********************************************/


/*******************************************************************************
*�������ƣ�IICInit
*����˵����IIC��ʼ��
*�������ߣ�embest
*******************************************************************************/
void IIC_Config(void)
{
  SDAPxSEL &= ~SDAPORTBIT;  //DATA����Ϊͨ��IO��
  SCLPxSEL &= ~SDAPORTBIT;  //SCK����Ϊͨ��IO��
    
  SCLPxDIR  |= SCLPORTBIT;    //SCK����Ϊ���
  SDAPxDIR  |= SDAPORTBIT;    //DATA����Ϊ���
}


/************************************************************************
�������ƣ�IIC_SDA_IN
�������ܣ�����SDA�˿�Ϊ�����
�������Σ���
��������ֵ����
�������ߣ�2016-4 zxg
************************************************************************/
void IIC_SDA_IN(void)
{
  SDAPxDIR &= ~SDAPORTBIT ;
}

/************************************************************************
�������ƣ�IIC_SDA_OUT
�������ܣ�����SDA�˿�Ϊ�����
�������Σ���
��������ֵ����
�������ߣ�2016-4 zxg
************************************************************************/
void IIC_SDA_OUT(void)
{
  SDAPxDIR |= SDAPORTBIT;
}

void MicroWait( uint16 timeout )
{
  while (timeout--)
  {
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}
/***********************************************************
* ����: Delay1us()
* ����: ��ʱ1us����
* ����: ���� ��Ҫ��ʱ��΢����
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Delay1us(uint16 t)
{ 
    /*uint8 i;
    for(i = 30; i > 0; i--)
        ;
        //MicroWait(1);     //����Э��ջ�ڵĺ���
  */
  uint8 i;
    for(i = t; i > 0; i--)        
        MicroWait(1);     //����Э��ջ�ڵĺ���
}

/***********************************************************
* ����: delay_ms()
* ����: ������ʱ1ms����
* ����: ������Ҫ��ʱ�ĺ�����
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void delay_ms(uint16 ms)
{
  /* unsigned char i; 
    while(ms--)  
    { 
      for(i=0;i<100;i++)    
       Delay1us(10);  
    }
  */
  unsigned char i; 
    while(ms--)  
    { 
      for(i=0;i<100;i++)    
       MicroWait(10);  
    }
}
/***********************************************************
* ����: AM232XX_Init()
* ����: ��ʪ�ȴ�������ʼ������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void AM232XX_Init(void)
{
  IIC_Config();
  Set_IIC_SCL();
  Set_IIC_SDA(); 
}

//**********************************************
//����ʼλ sda=1->0
void I2C_Start(void)
{
  IIC_SDA_OUT();
  Set_IIC_SDA();
  Set_IIC_SCL();
  Delay1us(10);
  Clr_IIC_SDA();
  Delay1us(10);
  Clr_IIC_SCL(); 
}
//************************************************
//��ֹͣλ sda=0->1
void I2C_Stop(void)
{
  IIC_SDA_OUT();
  Clr_IIC_SDA();
  Delay1us(10);
  Set_IIC_SCL();
  Delay1us(10);
  Set_IIC_SDA();
}

//************************************************
//��Ӧ��(����ack:sda=0��no_ack:sda=0)
void Ack(void)
{  //����SDA ��Ϊ���
  IIC_SDA_OUT();
  Clr_IIC_SDA();
  Clr_IIC_SCL();
  Delay1us(10);
  Set_IIC_SCL();
  Delay1us(10);	
  Clr_IIC_SCL();
  Set_IIC_SDA();
}

void NoAck(void)
{  //����SDA ��Ϊ���
  IIC_SDA_OUT();
  Set_IIC_SDA();  
  Clr_IIC_SCL();
  Delay1us(10);
  Set_IIC_SCL();
  Delay1us(10);
  Set_IIC_SDA();
  Clr_IIC_SCL();
}

// ��� SDA�Ƿ��ACK
char Test_Ack(void)
{  //����SDA ��Ϊ����
  char ACK_Flag=0;
  IIC_SDA_IN();
  Clr_IIC_SCL();
  Set_IIC_SDA();   
  Delay1us(10);
  Set_IIC_SCL();
  Delay1us(10);
  if(READ_SDA()==0)
    ACK_Flag = 1;
  else 
    ACK_Flag = 0;
  Clr_IIC_SCL();
  return ACK_Flag;
}

//*************************************************
//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
void SendData(unsigned char buffer)
{
  unsigned char BitCnt=8;//һ�ֽ�8λ
  IIC_SDA_OUT();
  do
  {
    Clr_IIC_SCL();
    Delay1us(10);
    if((buffer&0x80)==0) //�ж����λ��0����1
      Clr_IIC_SDA();
    else
      Set_IIC_SDA();  
    Set_IIC_SCL();
    Delay1us(10);
    buffer=buffer<<1;//��buffer�е���������һλ
    BitCnt--;
  }
  while(BitCnt);
  Clr_IIC_SCL();      
}


//**************************************************
//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|i2c_ack_main()ʹ��
//return: uchar��1�ֽ�
unsigned char ReceiveData(void)
{ 
  unsigned char BitCnt=8,Data=0;
  Set_IIC_SDA();             //�������� ����SDA ��Ϊ����
  IIC_SDA_IN();
  do
  {
    Clr_IIC_SCL();  
    Delay1us(10);  
    Data=Data<<1;   //��������һλ
    BitCnt--;	  
    Set_IIC_SCL();
    Delay1us(10); 
    if(READ_SDA()==1)
      Data = Data|0x01;  //��λ��1
    else
      Data = Data&0x0fe; //��λ��0	    
  }
  while(BitCnt);
  Clr_IIC_SCL();  
  return Data;
}

//***************************************************
char WriteNByte(unsigned char sla,unsigned char *s,unsigned char n)
{
  unsigned char i;
  
  I2C_Start();  //����I2C
  SendData(sla);//����������ַ
  if(!Test_Ack())
  {	
    WR_Flag = 1;
    return(0);
  }
  for(i=0;i<n;i++)//д��8�ֽ�����
  {
    SendData(*(s+i));
    if(!Test_Ack())
    {
      WR_Flag = 1;
      return(0);
    }
  }
  I2C_Stop();
  return(1);
}

char ReadNByte(unsigned char Sal, unsigned char *p,unsigned char n)
{
  unsigned char i;
  I2C_Start();    // ����I2C
  SendData((Sal)| 0x01); //����������ַ
  if(!Test_Ack())
  {
  	WR_Flag = 1;
	return(0);
  }
  Delay1us(10);  
  Delay1us(10);
  Delay1us(10); // ��ʱʱ��������30us ֻҪ���� 30us ���ϵ�ֵ������ ������ò�Ҫ̫�� ������ʱ���Թ�25MS��OK�� 
        
  for(i=0;i<n-1;i++)  //��ȡ�ֽ�����
  {
     *(p+i)=ReceiveData(); //��ȡ����
     Ack(); 
  }
  *(p+n-1)=ReceiveData();        
  NoAck();
  I2C_Stop(); 
  return(1);	 
}

///����CRCУ����	
unsigned int CRC16(unsigned char *ptr, unsigned char len)
{
   unsigned int crc=0xffff;
   unsigned char i;
   while(len--)
   {
       crc ^=*ptr++;
       for(i=0;i<8;i++)
	   {
	       if(crc & 0x1)
		   {
		      crc>>=1;
			  crc^=0xa001;
		   }
		   else
		   {
		      crc>>=1;
		   }
	   }
   }
   return crc;
}
///���CRCУ�����Ƿ���ȷ
unsigned char CheckCRC(unsigned char *ptr,unsigned char len)
{
  unsigned int crc;
	crc=(unsigned int)CRC16(ptr,len-2);
	if(ptr[len-1]==(crc>>8) && ptr[len-2]==(crc & 0x00ff))
	{
	    return 0xff;
	}
	else
	{
	   return 0x0;
	}
}


void Waken(void)
{
  I2C_Start();       // ����I2C
  SendData(IIC_Add); // ����������ַ
  Test_Ack();	       // ����ָ��ʱ �����������ACK ���ǵ�һ��Ҫ�����ACK��ʱ�� ��������
  delay_ms(2);       // ������ʱ1��Ms	˵������и����ֵ ��ʵ�ʵ��� ��ֻҪ����1MS
  I2C_Stop();	
}


void Get_Temp(uint8 *temp,uint8 *humi)
{
  WR_Flag = 0;
  Waken();	  // ���Ѵ�����
  //���Ͷ�ָ��
  WriteNByte(IIC_Add,IIC_CMD,3); 
  //���Ͷ�ȡ��д������������ٵȴ�2MS����̽ͷ����������ʱ��׼����
  delay_ms(2);    
  //����������
  ReadNByte(IIC_Add,Buffer,8);
  
  Set_IIC_SCL();
  Set_IIC_SDA();//ȷ���ͷ�����
  
  if(WR_Flag == 0)
  {
    if(CheckCRC(Buffer,8))
    { 
      *humi = (Buffer[2]*256+Buffer[3])/10;
      *temp = (Buffer[4]*256+Buffer[5])/10;	   
    }
  }    
}


