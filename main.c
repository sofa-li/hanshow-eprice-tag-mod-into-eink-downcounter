#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "font.h"
#include "epd.h"
#define epdon P2OUT &= 0xbf
#define epdoff  P2OUT |= 0x40
#define spiromcshigh P3OUT |= 0x01
#define spiromcslow P3OUT &= 0xfe
#define epdunknowhigh P3OUT &= 0x80
#define epdbslow P3OUT &= 0xfd
static void Init(void);
void delay(uint16_t nClk);
void initTimer_A(void);
void display(uint16_t days);
int daydayup=155;   //倒计时的起点
uint16_t day24=2009;   //凑24小时的计数器，例如每次终端最长是43s，24小时需要2010个43s
int main(void)
{
  Init();   //系统初始化
  EPD_init();   //屏幕初始化
  PIC_display_Clean();   //屏幕内容清掉
  EPD_sleep();         //必须加上
  EPD_partial_display_Full(0,0,gImage_juli,212,104,0);   //显示底图
  display(daydayup);    //局部刷新倒计时起点
  EPD_sleep();
  TA0CCR0 = 65532;//overflow freq = 1500/(TACCR0+1) ，启动计时器的最大值2^16
  TA0CCTL0 = CCIE; //enable interrupt
  TA0CTL = TASSEL_1 + MC_1 +TACLR; //ACLK, no divide, up mode ,clear timer
   //the actual overflow period measured: 585~595ms -> VLO freq is ~10k
   //so the VLO is not very accurate.
  _enable_interrupt();
  LPM3;
//  while(1);
}

void Init(void)
{
  WDTCTL = WDT_ARST_1000 + WDTHOLD; // stop watchdog timer
  //SYS CLK
  DCOCTL = 0x0000;
  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL = CALDCO_1MHZ;

  BCSCTL3 |= LFXT1S_2;                   /* Mode 2 for LFXT1 : VLO */
  BCSCTL1 |= DIVA_3;                    //ACLK = VLO divided by 8: typical 1.5kHz
  //GPIO for EPD
  P1OUT=0x00; //all set zero
  P1SEL=0x00; //set io function
  P1DIR=0xFF; // set p1 all out

  P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 |BIT6);  // set 0 1 2 3 4 6 zero
  P2OUT &= ~BIT6;  //set 6 zero
  P2OUT |= BIT7; //set 7 one
  P2SEL=0x00; //set io function
  P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 |BIT4 | BIT6 | BIT7); //set 0 1 2 3 4 6 7 output
  P2DIR &= ~BIT5;  //set 5 input

  P3OUT&= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 |BIT6);;  //all set zero
  P3OUT |= BIT7; //unknown pin to epd
  P3SEL=0x00;  //set io function
  P3DIR=0xFF;  //set p3 all out
}

void delay(uint16_t nClk)
{
    uint8_t i;
    uint16_t  j;
  for(i = nClk; i; i--)
      {
          for(j=1000;j;j--)
          {
              _NOP();
          }

      }
}

//ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void)
{
    if (day24)
    {
        day24=day24-1;

    }
    else
    {
        daydayup=daydayup-1;
        EPD_partial_display_Full(0,0,gImage_juli,212,104,0);
        display(daydayup);
        EPD_sleep();
        day24=2009;
    }
}


void display(uint16_t days)
{
    uint8_t a,b,c;

    if (days>=100)
      {
        a=days/100;
        b=(days-a*100)/10;
        c=days-a*100-b*10;
        EPD_partial_display_Full_3(56,50,gImage_num[c],gImage_num[b],gImage_num[a],96,32,0);
      }
      else if (days<100 & days>=10)
      {
          b=days/10;
          c=days-b*10;
          EPD_partial_display_Full_2(56,66,gImage_num[b],gImage_num[b],64,32,0);
      }
      else
      {
          c=days;
          EPD_partial_display_Full(56,82,gImage_num[c],32,32,0);
      }
}
