//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : main.c
//  ��������   : OLED �ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              VCC  ��5V��3.3v��Դ
//              GND  ��Դ��
//              D1   ��PB15��SDA��
//              D0   ��PB13��SCL��
//              CS   ��PB12    
//              RST  ��ϵͳ��λ
//              DC   ��PC6
//              NC   ����          
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "stdlib.h"
#include "stdint.h"
#define u8 uint8_t
#define u32 uint32_t
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���---------------- 

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOA, RS_Pin, GPIO_PIN_RESET)//RS
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOA, RS_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOC, D_C_Pin, GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOC, D_C_Pin, GPIO_PIN_SET)

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET)

 
//PC0~7,��Ϊ������
#define DATAOUT(x) HAL_GPIO_WritePin(GPIOC, x, GPIO_PIN_SET);//���  
//ʹ��4�ߴ��нӿ�ʱʹ�� 


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



