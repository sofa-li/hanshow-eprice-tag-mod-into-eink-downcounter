#ifndef EPD_H_
#define EPD_H_

#define EPD_HEIGHT 250
#define EPD_WIDTH  122

void EPD_init();
void EPD_dispUpdate(uint8_t full);
void EPD_busyWait();
bool EPD_isBusy();
void EPD_sleep(void);
void EPD_draw(uint8_t x, uint8_t y, uint8_t xsize, uint8_t ysize, const uint8_t *dat);
void lcd_chkstatus();
void PIC_display_Clean();
void EPD_partial_display_Full(uint8_t x_start,uint8_t y_start,const unsigned char *new_data,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode);
void EPD_partial_display_Full_2(uint8_t x_start,uint8_t y_start,const unsigned char *new_data_1,const unsigned char *new_data_2,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode);
void EPD_partial_display_Full_3(uint8_t x_start,uint8_t y_start,const unsigned char *new_data_1,const unsigned char *new_data_2,const unsigned char *new_data_3,uint8_t PART_COLUMN,uint8_t PART_LINE,unsigned char mode);
void EPD_init_data();//全刷初始化，不复位
void EPD_refresh();//EPD_refresh
#endif /* EPD_H_ */
