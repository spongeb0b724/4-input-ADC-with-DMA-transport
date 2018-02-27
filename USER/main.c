#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"

#define Intput_Channel 4  												//输入通道数
 
 int main(void)
 {	 
  uint32_t adcx;
	 u8 x,y,i;
	float temp;
	delay_init();	    	 														//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	KEY_Init();																			//按键初始化	
 	LED_Init();			    											 			//LED端口初始化
	LCD_Init();			 																//LCD初始化
 	ADC_Initializes();		  												//ADC初始化

	while(1)
	{
		delay_ms(500);															//延时500ms
		LED0=!LED0;																	//LED亮灭变化
		
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //启动转换
		while(!DMA_GetFlagStatus(DMA1_IT_TC1));     //等待传输完成
		DMA_ClearFlag(DMA1_IT_GL1);                 //清除标志位
		
		x=110;
		y=130;
		POINT_COLOR=BLUE;														//设置字体为蓝色
		for(i=0;i<Intput_Channel;i++)
		{
	LCD_ShowString(60,x,200,16,16,"ADC_CHX_VAL:");	      
	LCD_ShowString(60,y,200,16,16,"ADC_CHX_VOL:0.000V");	 
	LCD_ShowxNum(108,x,i+1,1,16,0);  
	LCD_ShowxNum(108,y,i+1,1,16,0);	
		adcx=ADC_Convert(ADC_Buf[i]);
		LCD_ShowxNum(156,x,adcx,4,16,0);					//显示ADC的值
		temp=(float)adcx*(3.3/4096);    					//参考电压3.3v
		adcx=temp;
		LCD_ShowxNum(156,y,adcx,1,16,0);					//显示电压值
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,y,temp,3,16,0X80);
		x+=40;
		y+=40;
		delay_ms(300);	
		}
		LED0=!LED0;
		delay_ms(250);	
	}
 }

