#include <ioCC2530.h>             //����CC2530����Ӧ��ͷ�ļ���������SFR�Ķ��壩

/*----------------------------------------------------------------------------*/
/*                            macro declaration 	                      */
/*----------------------------------------------------------------------------*/

#define uint   unsigned  int
#define uchar  unsigned  char

#define CLKSPD  ( CLKCONCMD & 0x07 )    //getting the clock division factor

void xtal_init(void);
void uart0_init(unsigned char StopBits,unsigned char Parity);
