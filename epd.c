#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "epd.h"
#include "lut.h"

#define EPD_W21_CK_LOW P2OUT &= ~BIT3
#define EPD_W21_CK_HIGH P2OUT |= BIT3
#define EPD_W21_DA_LOW P2OUT &= ~BIT4
#define EPD_W21_DA_HIGH P2OUT |= BIT4
#define EPD_W21_CS_LOW P3OUT &= ~BIT4
#define EPD_W21_CS_HIGH P3OUT |= BIT4
#define EPD_W21_DC_LOW P3OUT &= ~BIT5
#define EPD_W21_DC_HIGH P3OUT |= BIT5
#define EPD_W21_RST_LOW P3OUT &= ~BIT6
#define EPD_W21_RST_HIGH P3OUT |= BIT6
#define isEPD_W21_BUSY (BIT5 & P2IN)
#define epdon           P2OUT &= 0xbf
#define epdoff          P2OUT |= 0x40

static void EPD_writeByte(uint8_t);
static void EPD_W21_WriteCMD(uint8_t);
static void EPD_W21_WriteDATA(uint8_t);
//static void EPD_W21_WriteCMD1DAT(uint8_t cmd, uint8_t);
//static void EPD_W21_WriteCMD2DAT(uint8_t cmd, uint8_t, uint8_t);
extern void delay(uint16_t nClk);

//void EPD_init()
//{
// // EPD_W21_CS_HIGH;
// //EPD_W21_CK_LOW;
//
////  uint8_t HRES=0x68;                        //104
////  uint8_t VRES_byte1=0x00;          //212
////  uint8_t VRES_byte2=0xd4;
//  EPD_W21_RST_LOW;
//  delay(100);
//  EPD_W21_RST_HIGH;
//  delay(100);
//  lcd_chkstatus();
//
//
//  EPD_W21_WriteCMD(0x01);
//  EPD_W21_WriteDATA(0x03);
//  EPD_W21_WriteDATA(0x00);
//  EPD_W21_WriteDATA(0x2b);
//  EPD_W21_WriteDATA(0x2b);
//  EPD_W21_WriteDATA(0x03);
//
//    EPD_W21_WriteCMD(0x06);
//    EPD_W21_WriteDATA(0x17);
//    EPD_W21_WriteDATA(0x17);
//    EPD_W21_WriteDATA(0x17);
//
//  EPD_W21_WriteCMD(0x04);
//  lcd_chkstatus();
//
//  EPD_W21_WriteCMD(0x00);
//  EPD_W21_WriteDATA(0xbf);
//  EPD_W21_WriteDATA(0x0d);
//
//  EPD_W21_WriteCMD(0x30);
//  EPD_W21_WriteDATA(0x3c);
//
//  EPD_W21_WriteCMD(0x61);
//  EPD_W21_WriteDATA(0x68);
//  EPD_W21_WriteDATA(0x00);
//  EPD_W21_WriteDATA(0xd4);
//
//  EPD_W21_WriteCMD(0x82);
//  EPD_W21_WriteDATA(0x12);
//
//  EPD_W21_WriteCMD(0x50);
//  EPD_W21_WriteDATA(0x97);
//
//}
void EPD_init()
{
 // EPD_W21_CS_HIGH;
 //EPD_W21_CK_LOW;
  EPD_W21_RST_LOW;
  delay(100);
  EPD_W21_RST_HIGH;
  delay(100);
  lcd_chkstatus();


  EPD_W21_WriteCMD(0x06);   //soft start
  EPD_W21_WriteDATA(0x17);  //a
  EPD_W21_WriteDATA(0x17);  //b
  EPD_W21_WriteDATA(0x17);  //c

  EPD_W21_WriteCMD(0x04);   //power on
  lcd_chkstatus();          //waiting for the elec

  EPD_W21_WriteCMD(0x00);   //panel setting
  EPD_W21_WriteDATA(0x1f);  //lut from otp
  EPD_W21_WriteDATA(0x0d);  //vcom to 0v fast

  EPD_W21_WriteCMD(0x61);   //resolution setting
  EPD_W21_WriteDATA(0x98);  //152
  EPD_W21_WriteDATA(0x00);
  EPD_W21_WriteDATA(0xd4);  //252

  EPD_W21_WriteCMD(0x50);   //vcom and data in
  EPD_W21_WriteDATA(0x97);  //wbmod:wbdf 17

  //  EPD_W21_WriteCMD(0x82);
  //  EPD_W21_WriteDATA(0x0A);
}
//void EPD_init()
//{
// // EPD_W21_CS_HIGH;
// //EPD_W21_CK_LOW;
//  EPD_W21_RST_LOW;
//  delay(100);
//  EPD_W21_RST_HIGH;
//  delay(100);
//  lcd_chkstatus();
//
//
//  EPD_W21_WriteCMD(0x01);   //soft start
//  EPD_W21_WriteDATA(0xf9);
//  EPD_W21_WriteDATA(0x00);
////  EPD_W21_WriteDATA(0x17);
//
//  EPD_W21_WriteCMD(0x3a);
//  EPD_W21_WriteDATA(0x06);
//  EPD_W21_WriteCMD(0x3b);
//  EPD_W21_WriteDATA(0x0b);
//  EPD_W21_WriteCMD(0x11);
//  EPD_W21_WriteDATA(0x06);
//
//  EPD_W21_WriteCMD(0x44);
//  EPD_W21_WriteDATA(0x00);
//  EPD_W21_WriteDATA(0x0f);
//
//  EPD_W21_WriteCMD(0x45);
//  EPD_W21_WriteDATA(0x00);
//  EPD_W21_WriteDATA(0xf9);
//
//
//  EPD_W21_WriteCMD(0x2c);
//  EPD_W21_WriteDATA(0xa0);
//  EPD_W21_WriteCMD(0x3c);
//  EPD_W21_WriteDATA(0x33);
//  EPD_W21_WriteCMD(0x21);
//  EPD_W21_WriteDATA(0x0d);
//
//  //  EPD_W21_WriteCMD(0x82);
//  //  EPD_W21_WriteDATA(0x0A);
//}

void EPD_init_data(void)
{
    uint8_t HRES=0x68;                        //104
    uint8_t VRES_byte1=0x00;          //212
//    uint8_t VRES_byte2=0xd4;

        EPD_W21_WriteCMD(0x04);
        lcd_chkstatus();

        EPD_W21_WriteCMD(0x00);         //panel setting
        EPD_W21_WriteDATA(0x1f);        //LUT from OTP，128x296

        EPD_W21_WriteCMD(0x61);         //resolution setting
        EPD_W21_WriteDATA (HRES);
        EPD_W21_WriteDATA (VRES_byte1);
        EPD_W21_WriteDATA (0xd4);

        EPD_W21_WriteCMD(0x50);         //VCOM AND DATA INTERVAL SETTING
        EPD_W21_WriteDATA(0x17);        //WBmode:VBDF 17|D7 VBDW 97 VBDB 57     WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void EPD_refresh(void)
{
        EPD_W21_WriteCMD(0x12);         //DISPLAY REFRESH
        delay(100);          //!!!The delay here is necessary, 200uS at least!!!
        lcd_chkstatus();
}

void EPD_sleep(void)
{
        EPD_W21_WriteCMD(0X50);
        EPD_W21_WriteDATA(0xf7);
        EPD_W21_WriteCMD(0X02);     //power off
        lcd_chkstatus();
        EPD_W21_WriteCMD(0X07);     //deep sleep
        EPD_W21_WriteDATA(0xA5);

}
void PIC_display_Clean(void)
{
    uint16_t i;
        EPD_W21_WriteCMD(0x10);        //Transfer old data
      for(i=4100;i;i--)
    {
      EPD_W21_WriteDATA(0xff);
    }

        EPD_W21_WriteCMD(0x13);          //Transfer new data
      for(i=4100;i;i--)
    {
      EPD_W21_WriteDATA(0xff);
    }
    EPD_refresh();
}
void lcd_chkstatus()
{
//    unsigned char busy;
//    do
//    {
//        EPD_W21_WriteCMD(0x71);
//        busy = isEPD_W21_BUSY;
//        busy =!(busy & 0x01);
//    }
//    while(busy);
    while(!isEPD_W21_BUSY);
    delay(200);
}

//void EPD_dispUpdate(uint8_t full)
//{
//  const uint8_t *lut;
//  uint16_t i;
//
//  EPD_W21_CS_LOW;
//  if (full)
//  {
//    lut = LUT;
//  }
//  else
//  {
//    lut = LUT_partial;
//  }
//  EPD_W21_WriteCMD(0x32); //Write LUT register 29 bytes
//  EPD_W21_DC_HIGH;
//  for (i = 29; i; i--)
//  {
//    EPD_writeByte(*lut++);
//  }
//  EPD_W21_WriteCMD1DAT(0x22, 0xc7); //Display Update Control 2 + Setting for LUT from MCU
//  EPD_W21_WriteCMD(0x20);           //Master Activation
//  EPD_W21_CS_HIGH;
//}

//static void epd_setWindow(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end)
//{
//  EPD_W21_WriteCMD2DAT(0x44, x_start >> 3, (x_end + 7) >> 3); // x point must be the multiple of 8 or the last 3 bits will be ignored
//  EPD_W21_WriteCMD2DAT(0x45, y_start, y_end);
//}
//static void epd_setCursor(uint8_t x, uint8_t y)
//{
//  EPD_W21_WriteCMD1DAT(0x4E, x >> 3); // x point must be the multiple of 8 or the last 3 bits will be ignored
//  EPD_W21_WriteCMD1DAT(0x4F, y);
//}
//void EPD_draw(uint8_t x, uint8_t y, uint8_t xsize, uint8_t ysize, const uint8_t *dat)
//{
//  uint16_t length = (xsize + 7) / 8 * ysize;
//  EPD_W21_CS_LOW;
//  epd_setWindow(x, y, x + xsize - 1, y + ysize - 1);
//  epd_setCursor(x + xsize - 1, y);
//  EPD_W21_WriteCMD(0x24); //write ram
//  EPD_W21_DC_HIGH;
//  if (dat)
//  {
//    do
//    {
//      EPD_writeByte(*dat++);
//    } while (--length);
//  }
//  else
//  {
//    do
//    {
//      EPD_writeByte(0);
//    } while (--length);
//  }
//  EPD_W21_CS_HIGH;
//}

void EPD_busyWait()
{
  delay(10);
  while (isEPD_W21_BUSY)
    ;
}

bool EPD_isBusy()
{
  return isEPD_W21_BUSY;
}

void EPD_writeByte(uint8_t sdbyte)
{
  uint16_t i;
  _NOP();
  for (i = 8; i; i--)
  {
    EPD_W21_CK_LOW;
    _NOP();
    if (sdbyte & 0x80)
    {
      EPD_W21_DA_HIGH;
    }
    else
    {
      EPD_W21_DA_LOW;
    }
    sdbyte <<= 1;
    _NOP();
    EPD_W21_CK_HIGH;
    _NOP();

  }
}

static void EPD_W21_WriteCMD(unsigned char command)
{
    _NOP();
    EPD_W21_CS_LOW;
    EPD_W21_DC_LOW;
    EPD_writeByte(command);
    EPD_W21_CS_HIGH;
}
static void EPD_W21_WriteDATA(unsigned char dat)
{
    _NOP();
    EPD_W21_CS_LOW;
    EPD_W21_DC_HIGH;
    EPD_writeByte(dat);
    EPD_W21_CS_HIGH;
}

//static void EPD_W21_WriteCMD1DAT(uint8_t cmd, uint8_t data)
//{
//  EPD_W21_WriteCMD(cmd);
//  EPD_W21_WriteDATA(data);
//}
//static void EPD_W21_WriteCMD2DAT(uint8_t cmd, uint8_t data, uint8_t data2)
//{
//  EPD_W21_WriteCMD(cmd);
//  EPD_W21_WriteDATA(data);
//  EPD_writeByte(data2);
//}


void EPD_partial_display_Full(uint8_t x_start,uint8_t y_start,const unsigned char *new_data,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode) //partial display
{
    unsigned int i,count;
    unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
    x_start=x_start;//转换为字节
    x_end=x_start+PART_LINE-1;

    y_start1=0;
    y_start2=y_start;
    if(y_start>=256)
    {
        y_start1=y_start2/256;
        y_start2=y_start2%256;
    }
    y_end1=0;
    y_end2=y_start+PART_COLUMN-1;
    if(y_end2>=256)
    {
        y_end1=y_end2/256;
        y_end2=y_end2%256;
    }

  count=PART_COLUMN*PART_LINE/8;

      EPD_init(); //EPD init
      EPD_W21_WriteCMD(0x91);       //This command makes the display enter partial mode
        EPD_W21_WriteCMD(0x90);     //resolution setting
        EPD_W21_WriteDATA (x_start);   //x-start
        EPD_W21_WriteDATA (x_end);   //x-end

        EPD_W21_WriteDATA (y_start1);
        EPD_W21_WriteDATA (y_start2);   //y-start

        EPD_W21_WriteDATA (y_end1);
        EPD_W21_WriteDATA (y_end2);  //y-end
        EPD_W21_WriteDATA (0x28);

        EPD_W21_WriteCMD(0x10);        //writes Old data to SRAM for programming

    for(i=0;i<count;i++)
    {
     EPD_W21_WriteDATA(0x00);
    }

    if(mode==0)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(new_data[i]); //图片
     }
 }
    else if(mode==1)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0xff);  //全白
     }
 }
    else if(mode==2)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0x00);  //全黑
     }
 }
//    EPD_W21_WriteCMD(0x11);  //data stop

   EPD_init_data();//全刷初始化，不复位
   EPD_refresh();//EPD_refresh


}

void EPD_partial_display_Full_2(uint8_t x_start,uint8_t y_start,const unsigned char *new_data_1,const unsigned char *new_data_2,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode) //partial display
{
    unsigned int i,count,count2;
    unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
    x_start=x_start;//转换为字节
    x_end=x_start+PART_LINE-1;

    y_start1=0;
    y_start2=y_start;
    if(y_start>=256)
    {
        y_start1=y_start2/256;
        y_start2=y_start2%256;
    }
    y_end1=0;
    y_end2=y_start+PART_COLUMN-1;
    if(y_end2>=256)
    {
        y_end1=y_end2/256;
        y_end2=y_end2%256;
    }

  count=PART_COLUMN*PART_LINE/8;

      EPD_init(); //EPD init
      EPD_W21_WriteCMD(0x91);       //This command makes the display enter partial mode
        EPD_W21_WriteCMD(0x90);     //resolution setting
        EPD_W21_WriteDATA (x_start);   //x-start
        EPD_W21_WriteDATA (x_end);   //x-end

        EPD_W21_WriteDATA (y_start1);
        EPD_W21_WriteDATA (y_start2);   //y-start

        EPD_W21_WriteDATA (y_end1);
        EPD_W21_WriteDATA (y_end2);  //y-end
        EPD_W21_WriteDATA (0x28);

        EPD_W21_WriteCMD(0x10);        //writes Old data to SRAM for programming
    count2=PART_COLUMN*PART_LINE/8/2;
    for(i=0;i<count;i++)
    {
     EPD_W21_WriteDATA(0x00);
    }

    if(mode==0)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count2;i++)
     {
        EPD_W21_WriteDATA(new_data_1[i]); //图片
     }
        for(i=0;i<count2;i++)
     {
        EPD_W21_WriteDATA(new_data_2[i]); //图片
     }
    }
    else if(mode==1)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0xff);  //全白
     }
    }
    else if(mode==2)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0x00);  //全黑
     }
 }
//    EPD_W21_WriteCMD(0x11);  //data stop

   EPD_init_data();//全刷初始化，不复位
   EPD_refresh();//EPD_refresh


}
void EPD_partial_display_Full_3(uint8_t x_start,uint8_t y_start,const unsigned char *new_data_1,const unsigned char *new_data_2,const unsigned char *new_data_3,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode) //partial display
{
    unsigned int i,count,count2;
    unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
    x_start=x_start;//转换为字节
    x_end=x_start+PART_LINE-1;

    y_start1=0;
    y_start2=y_start;
    if(y_start>=256)
    {
        y_start1=y_start2/256;
        y_start2=y_start2%256;
    }
    y_end1=0;
    y_end2=y_start+PART_COLUMN-1;
    if(y_end2>=256)
    {
        y_end1=y_end2/256;
        y_end2=y_end2%256;
    }

  count=PART_COLUMN*PART_LINE/8;

      EPD_init(); //EPD init
      EPD_W21_WriteCMD(0x91);       //This command makes the display enter partial mode
        EPD_W21_WriteCMD(0x90);     //resolution setting
        EPD_W21_WriteDATA (x_start);   //x-start
        EPD_W21_WriteDATA (x_end);   //x-end

        EPD_W21_WriteDATA (y_start1);
        EPD_W21_WriteDATA (y_start2);   //y-start

        EPD_W21_WriteDATA (y_end1);
        EPD_W21_WriteDATA (y_end2);  //y-end
        EPD_W21_WriteDATA (0x28);

        EPD_W21_WriteCMD(0x10);        //writes Old data to SRAM for programming
    count2=PART_COLUMN*PART_LINE/8/3;
    for(i=0;i<count;i++)
    {
     EPD_W21_WriteDATA(0x00);
    }

    if(mode==0)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count2;i++)
     {
        EPD_W21_WriteDATA(new_data_1[i]); //图片
     }
        for(i=0;i<count2;i++)
     {
        EPD_W21_WriteDATA(new_data_2[i]); //图片
     }
        for(i=0;i<count2;i++)
     {
        EPD_W21_WriteDATA(new_data_3[i]); //图片
     }
 }
    else if(mode==1)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0xff);  //全白
     }
 }
    else if(mode==2)
    {
    EPD_W21_WriteCMD(0x13);              //writes New data to SRAM.
        for(i=0;i<count;i++)
     {
        EPD_W21_WriteDATA(0x00);  //全黑
     }
 }
//    EPD_W21_WriteCMD(0x11);  //data stop

   EPD_init_data();//全刷初始化，不复位
   EPD_refresh();//EPD_refresh


}
