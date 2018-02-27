 #include "adc.h"
 #include "delay.h"
 /* 变量 ----------------------------------------------------------------------*/
uint16_t ADC_Buf[ADC_BUF_SIZE];
/*******************************************************
函数名称		： 	ADC1_GPIO_Configuration
功		能	：	ADC1引脚配置
参		数	：	无
返 回 值		：	无
作		者	：	SpongeB0B
********************************************************/
void ADC1_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE);	 
	//PA1~PA4作为4路模拟通道输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       //模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
/*******************************************************
函数名称		： 	ADC1_DMA_Configuration
功		能	：	ADC1_DMA配置
参		数	：	无
返 回 值		：	无
作		者	：	SpongeB0B
********************************************************/
void	ADC1_DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure; 
	
    DMA_DeInit(DMA1_Channel1);   																				//外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADC1->DR));  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Buf;//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 									//传输方向：外设->内存
    DMA_InitStructure.DMA_BufferSize =ADC_BUF_SIZE; 										//DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	//外设递增：关闭
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 					  //内存递增：打开
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ;  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度：16位
    DMA_InitStructure.DMA_Mode =DMA_Mode_Circular;  										//循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; 								//优先级：高 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  											//内存->内存：否
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
    DMA_Cmd(DMA1_Channel1,ENABLE);																			//使能通道
}

/*******************************************************
函数名称		： 	ADC1_Configuration
功		能	：	ADC1配置
参		数	：	无
返 回 值		：	无
作		者	：	SpongeB0B
********************************************************/																   
void  ADC1_Configuration(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	/*ADC1配置*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;												//浏览模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//ADC工作在单次转化模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 4;															//4个通道
	ADC_Init(ADC1, &ADC_InitStructure);	  
/********************************************************		
		设置指定ADC的规则通道，设置通道对应的转化顺序和采样时间
********************************************************/			
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5 );	//采样时间为55  			    
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5 );
	
	ADC_DMACmd(ADC1,ENABLE); 																						//使能ADC1的DMA功能
	ADC_Cmd(ADC1, ENABLE);																							//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);																					//复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));													//等待复位完成
	
	ADC_StartCalibration(ADC1);	 																				//开启ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));	 														//等待校准完成
}				  

/*******************************************************
函数名称		： 	ADC_Initializes
功		能	：	ADC初始化
参		数	：	无
返 回 值		：	无
作		者	：	SpongeB0B
********************************************************/	
void ADC_Initializes(void)
{
  ADC1_GPIO_Configuration();
  ADC1_Configuration();
  ADC1_DMA_Configuration();
}

/************************************************
函数名称 ： ADC_Convert
功    能 ： 电压转换
参    数 ： DRValue --- 数据寄存器值
返 回 值 ： 1000倍电压值
作    者 ： SpongeB0B
*************************************************/
uint32_t ADC_Convert(uint32_t DRValue)
{
  uint32_t ResultVolt = 0;

  ResultVolt = (DRValue*3300) / 4095;                                //计算得到1000倍的电压值

  return ResultVolt;                                                 //比如: 实际电压为1.25V, 返回的数据为1250
}
