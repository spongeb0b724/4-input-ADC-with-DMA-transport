 #include "adc.h"
 #include "delay.h"
 /* ���� ----------------------------------------------------------------------*/
uint16_t ADC_Buf[ADC_BUF_SIZE];
/*******************************************************
��������		�� 	ADC1_GPIO_Configuration
��		��	��	ADC1��������
��		��	��	��
�� �� ֵ		��	��
��		��	��	SpongeB0B
********************************************************/
void ADC1_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE);	 
	//PA1~PA4��Ϊ4·ģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       //ģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
/*******************************************************
��������		�� 	ADC1_DMA_Configuration
��		��	��	ADC1_DMA����
��		��	��	��
�� �� ֵ		��	��
��		��	��	SpongeB0B
********************************************************/
void	ADC1_DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure; 
	
    DMA_DeInit(DMA1_Channel1);   																				//�����ַ
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADC1->DR));  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Buf;//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 									//���䷽������->�ڴ�
    DMA_InitStructure.DMA_BufferSize =ADC_BUF_SIZE; 										//DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	//����������ر�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 					  //�ڴ��������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ;  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ�ȣ�16λ
    DMA_InitStructure.DMA_Mode =DMA_Mode_Circular;  										//ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; 								//���ȼ����� 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  											//�ڴ�->�ڴ棺��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
    DMA_Cmd(DMA1_Channel1,ENABLE);																			//ʹ��ͨ��
}

/*******************************************************
��������		�� 	ADC1_Configuration
��		��	��	ADC1����
��		��	��	��
�� �� ֵ		��	��
��		��	��	SpongeB0B
********************************************************/																   
void  ADC1_Configuration(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	/*ADC1����*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;												//���ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//ADC�����ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 4;															//4��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);	  
/********************************************************		
		����ָ��ADC�Ĺ���ͨ��������ͨ����Ӧ��ת��˳��Ͳ���ʱ��
********************************************************/			
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5 );	//����ʱ��Ϊ55  			    
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5 );
	
	ADC_DMACmd(ADC1,ENABLE); 																						//ʹ��ADC1��DMA����
	ADC_Cmd(ADC1, ENABLE);																							//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);																					//��λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));													//�ȴ���λ���
	
	ADC_StartCalibration(ADC1);	 																				//����ADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));	 														//�ȴ�У׼���
}				  

/*******************************************************
��������		�� 	ADC_Initializes
��		��	��	ADC��ʼ��
��		��	��	��
�� �� ֵ		��	��
��		��	��	SpongeB0B
********************************************************/	
void ADC_Initializes(void)
{
  ADC1_GPIO_Configuration();
  ADC1_Configuration();
  ADC1_DMA_Configuration();
}

/************************************************
�������� �� ADC_Convert
��    �� �� ��ѹת��
��    �� �� DRValue --- ���ݼĴ���ֵ
�� �� ֵ �� 1000����ѹֵ
��    �� �� SpongeB0B
*************************************************/
uint32_t ADC_Convert(uint32_t DRValue)
{
  uint32_t ResultVolt = 0;

  ResultVolt = (DRValue*3300) / 4095;                                //����õ�1000���ĵ�ѹֵ

  return ResultVolt;                                                 //����: ʵ�ʵ�ѹΪ1.25V, ���ص�����Ϊ1250
}
