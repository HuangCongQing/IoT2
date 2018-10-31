#include <ioCC2530.h>             //引入CC2530所对应的头文件（包含各SFR的定义）

/*----------------------------------------------------------------------------*/
/*                            macro declaration 	                      */
/*----------------------------------------------------------------------------*/

#define uint   unsigned  int
#define uchar  unsigned  char

#define CLKSPD  ( CLKCONCMD & 0x07 )    //getting the clock division factor

void xtal_init(void);
void uart0_init(unsigned char StopBits,unsigned char Parity);
