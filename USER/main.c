#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"

#define Intput_Channel 4  												//����ͨ����
 
 int main(void)
 {	 
  uint32_t adcx;
	 u8 x,y,i;
	float temp;
	delay_init();	    	 														//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	KEY_Init();																			//������ʼ��	
 	LED_Init();			    											 			//LED�˿ڳ�ʼ��
	LCD_Init();			 																//LCD��ʼ��
 	ADC_Initializes();		  												//ADC��ʼ��

	while(1)
	{
		delay_ms(500);															//��ʱ500ms
		LED0=!LED0;																	//LED����仯
		
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //����ת��
		while(!DMA_GetFlagStatus(DMA1_IT_TC1));     //�ȴ��������
		DMA_ClearFlag(DMA1_IT_GL1);                 //�����־λ
		
		x=110;
		y=130;
		POINT_COLOR=BLUE;														//��������Ϊ��ɫ
		for(i=0;i<Intput_Channel;i++)
		{
	LCD_ShowString(60,x,200,16,16,"ADC_CHX_VAL:");	      
	LCD_ShowString(60,y,200,16,16,"ADC_CHX_VOL:0.000V");	 
	LCD_ShowxNum(108,x,i+1,1,16,0);  
	LCD_ShowxNum(108,y,i+1,1,16,0);	
		adcx=ADC_Convert(ADC_Buf[i]);
		LCD_ShowxNum(156,x,adcx,4,16,0);					//��ʾADC��ֵ
		temp=(float)adcx*(3.3/4096);    					//�ο���ѹ3.3v
		adcx=temp;
		LCD_ShowxNum(156,y,adcx,1,16,0);					//��ʾ��ѹֵ
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

