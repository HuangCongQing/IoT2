/********************************************************************************
* Copyright (c) 2015，联创中控（北京）科技有限公司
* All rights reserved.
*
* 文件名称：AM2312b.c
* 摘	要：温室大棚区功能函数
*
* 当前版本：V1.0
* 作	者：张小刚
* 完成日期：2016年4月7日
* 修改摘要：
********************************************************************************/

/*************头文件********************/
#include "AM2321b.h"

/**************AM2312b 传感器使用端口定义*******************/
#define SDA                    P1_6		//温度传感器数据引脚的定义
#define SDAPxSEL               P1SEL
#define SDAPxDIR               P1DIR
#define SDAPORTBIT             0x40

#define SCL                     P1_7		//温度传感器时钟引脚的定义
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

#define IIC_Add 0xB8    //器件地址
unsigned char IIC_CMD[]={0x03,0x00,0x04}; //读温湿度命令（无CRC校验）
unsigned char Buffer[8] = {0x00};
unsigned char WR_Flag;


/*****************本地函数声明************************/
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
*函数名称：IICInit
*函数说明：IIC初始化
*函数作者：embest
*******************************************************************************/
void IIC_Config(void)
{
  SDAPxSEL &= ~SDAPORTBIT;  //DATA口作为通用IO口
  SCLPxSEL &= ~SDAPORTBIT;  //SCK口作为通用IO口
    
  SCLPxDIR  |= SCLPORTBIT;    //SCK设置为输出
  SDAPxDIR  |= SDAPORTBIT;    //DATA设置为输出
}


/************************************************************************
函数名称：IIC_SDA_IN
函数功能：设置SDA端口为输入口
函数传参：无
函数返回值：无
函数作者：2016-4 zxg
************************************************************************/
void IIC_SDA_IN(void)
{
  SDAPxDIR &= ~SDAPORTBIT ;
}

/************************************************************************
函数名称：IIC_SDA_OUT
函数功能：设置SDA端口为输出口
函数传参：无
函数返回值：无
函数作者：2016-4 zxg
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
* 名称: Delay1us()
* 功能: 延时1us函数
* 参数: 输入 需要延时的微秒数
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void Delay1us(uint16 t)
{ 
    /*uint8 i;
    for(i = 30; i > 0; i--)
        ;
        //MicroWait(1);     //这是协议栈内的函数
  */
  uint8 i;
    for(i = t; i > 0; i--)        
        MicroWait(1);     //这是协议栈内的函数
}

/***********************************************************
* 名称: delay_ms()
* 功能: 粗略延时1ms函数
* 参数: 输入需要延时的毫秒数
* 返回: 无
* 修改:
* 注释: 
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
* 名称: AM232XX_Init()
* 功能: 温湿度传感器初始化函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void AM232XX_Init(void)
{
  IIC_Config();
  Set_IIC_SCL();
  Set_IIC_SDA(); 
}

//**********************************************
//送起始位 sda=1->0
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
//送停止位 sda=0->1
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
//主应答(包含ack:sda=0和no_ack:sda=0)
void Ack(void)
{  //设置SDA 口为输出
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
{  //设置SDA 口为输出
  IIC_SDA_OUT();
  Set_IIC_SDA();  
  Clr_IIC_SCL();
  Delay1us(10);
  Set_IIC_SCL();
  Delay1us(10);
  Set_IIC_SDA();
  Clr_IIC_SCL();
}

// 检测 SDA是否回ACK
char Test_Ack(void)
{  //设置SDA 口为输入
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
//字节发送程序
//发送c(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
void SendData(unsigned char buffer)
{
  unsigned char BitCnt=8;//一字节8位
  IIC_SDA_OUT();
  do
  {
    Clr_IIC_SCL();
    Delay1us(10);
    if((buffer&0x80)==0) //判断最高位是0还是1
      Clr_IIC_SDA();
    else
      Set_IIC_SDA();  
    Set_IIC_SCL();
    Delay1us(10);
    buffer=buffer<<1;//将buffer中的数据左移一位
    BitCnt--;
  }
  while(BitCnt);
  Clr_IIC_SCL();      
}


//**************************************************
//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|i2c_ack_main()使用
//return: uchar型1字节
unsigned char ReceiveData(void)
{ 
  unsigned char BitCnt=8,Data=0;
  Set_IIC_SDA();             //读入数据 设置SDA 口为输入
  IIC_SDA_IN();
  do
  {
    Clr_IIC_SCL();  
    Delay1us(10);  
    Data=Data<<1;   //数据左移一位
    BitCnt--;	  
    Set_IIC_SCL();
    Delay1us(10); 
    if(READ_SDA()==1)
      Data = Data|0x01;  //低位置1
    else
      Data = Data&0x0fe; //低位清0	    
  }
  while(BitCnt);
  Clr_IIC_SCL();  
  return Data;
}

//***************************************************
char WriteNByte(unsigned char sla,unsigned char *s,unsigned char n)
{
  unsigned char i;
  
  I2C_Start();  //启动I2C
  SendData(sla);//发送器件地址
  if(!Test_Ack())
  {	
    WR_Flag = 1;
    return(0);
  }
  for(i=0;i<n;i++)//写入8字节数据
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
  I2C_Start();    // 启动I2C
  SendData((Sal)| 0x01); //发送器件地址
  if(!Test_Ack())
  {
  	WR_Flag = 1;
	return(0);
  }
  Delay1us(10);  
  Delay1us(10);
  Delay1us(10); // 延时时间必须大于30us 只要大于 30us 以上的值都可以 但是最好不要太长 ，测试时，试过25MS都OK！ 
        
  for(i=0;i<n-1;i++)  //读取字节数据
  {
     *(p+i)=ReceiveData(); //读取数据
     Ack(); 
  }
  *(p+n-1)=ReceiveData();        
  NoAck();
  I2C_Stop(); 
  return(1);	 
}

///计算CRC校验码	
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
///检测CRC校验码是否正确
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
  I2C_Start();       // 启动I2C
  SendData(IIC_Add); // 发送器件地址
  Test_Ack();	       // 唤醒指令时 传感器不会回ACK 但是第一定要发检测ACK的时钟 否则会出错
  delay_ms(2);       // 至少延时1个Ms	说明书里，有个最大值 ，实际当中 你只要大于1MS
  I2C_Stop();	
}


void Get_Temp(uint8 *temp,uint8 *humi)
{
  WR_Flag = 0;
  Waken();	  // 唤醒传感器
  //发送读指令
  WriteNByte(IIC_Add,IIC_CMD,3); 
  //发送读取或写数据命令后，至少等待2MS（给探头返回数据作时间准备）
  delay_ms(2);    
  //读返回数据
  ReadNByte(IIC_Add,Buffer,8);
  
  Set_IIC_SCL();
  Set_IIC_SDA();//确认释放总线
  
  if(WR_Flag == 0)
  {
    if(CheckCRC(Buffer,8))
    { 
      *humi = (Buffer[2]*256+Buffer[3])/10;
      *temp = (Buffer[4]*256+Buffer[5])/10;	   
    }
  }    
}


