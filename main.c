#include "MDR32Fx.h"                    // Device header
#include "MDR32F9Qx_adc.h"              // Keil::Drivers:ADC
#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT
#include "MDR32F9Qx_config.h"           // Keil::Device:Startup


#define FIR_ON PORT_SetBits (MDR_PORTB, PORT_Pin_1)
#define FIR_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_1)

#define SEC_ON PORT_SetBits (MDR_PORTB, PORT_Pin_2)
#define SEC_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_2)

#define THR_ON PORT_SetBits (MDR_PORTB, PORT_Pin_0)
#define THR_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_0)

#define FOUR_ON PORT_SetBits (MDR_PORTA, PORT_Pin_5)
#define FOUR_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_5)

#define FTH_ON PORT_SetBits (MDR_PORTA, PORT_Pin_4)
#define FTH_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_4)

#define SXTH_ON PORT_SetBits (MDR_PORTA, PORT_Pin_7)
#define SXTH_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_7)

#define SCTH_ON PORT_SetBits (MDR_PORTC, PORT_Pin_1)
#define SCTH_OFF PORT_ResetBits (MDR_PORTC, PORT_Pin_1)

#define EITH_ON PORT_SetBits (MDR_PORTC, PORT_Pin_0)
#define EITH_OFF PORT_ResetBits (MDR_PORTC, PORT_Pin_0)

#define ADC_prescaler 455
#define WIN_SIZE 10
#define NUM_READ 10
void Init_LED(void)
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	
	PORT_InitTypeDef PORT_InitStruct;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStruct.PORT_OE = PORT_OE_OUT;
	PORT_InitStruct.PORT_Pin = PORT_Pin_7+PORT_Pin_4+PORT_Pin_5;                //указать пины, доинициализировать порты если надо 
	PORT_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PORT_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init(MDR_PORTA, &PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = PORT_Pin_0+PORT_Pin_2+PORT_Pin_1;
	PORT_Init(MDR_PORTB, &PORT_InitStruct);
	PORT_InitStruct.PORT_Pin = PORT_Pin_0+PORT_Pin_1;
	PORT_Init(MDR_PORTC, &PORT_InitStruct);
	
}
void Init_ADC()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);
	PORT_InitTypeDef PORT_InitStruct;
	PORT_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStruct.PORT_FUNC = PORT_FUNC_PORT;   //Нужно ли(не включит ли цифровой режим????7)
	PORT_InitStruct.PORT_MODE = PORT_MODE_ANALOG;
	PORT_InitStruct.PORT_OE = PORT_OE_IN;
	PORT_InitStruct.PORT_Pin = PORT_Pin_7;                
	PORT_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PORT_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init(MDR_PORTD, &PORT_InitStruct);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_SynchronousMode = ADC_SyncMode_Independent;
	ADC_InitStruct.ADC_StartDelay = 0;
	ADC_InitStruct.ADC_TempSensor = ADC_TEMP_SENSOR_AMPLIFIER_Disable;
	ADC_InitStruct.ADC_IntVRefConversion = ADC_VREF_CONVERSION_Disable;
	ADC_InitStruct.ADC_IntVRefTrimming = 1;
	ADC_Init(&ADC_InitStruct);
	
	ADCx_InitTypeDef ADC1_InitStruct;
	ADC1_InitStruct.ADC_ClockSource = ADC_CLOCK_SOURCE_CPU;
	ADC1_InitStruct.ADC_SamplingMode = ADC_SAMPLING_MODE_CICLIC_CONV;
	ADC1_InitStruct.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
	ADC1_InitStruct.ADC_ChannelNumber = ADC_CH_ADC7; //Точно ли этот канал??
	ADC1_InitStruct.ADC_Channels = 0;
	ADC1_InitStruct.ADC_LevelControl = ADC_LEVEL_CONTROL_Enable;
	ADC1_InitStruct.ADC_LowLevel = 0;
	ADC1_InitStruct.ADC_HighLevel = ADC_VALUE_MAX;
	ADC1_InitStruct.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
	ADC1_InitStruct.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
	ADC1_InitStruct.ADC_Prescaler = ADC_CLK_div_32768;
	ADC1_InitStruct.ADC_DelayGo = 0xF;
	ADC1_Init(&ADC1_InitStruct);
	
	ADC1_SetChannel(ADC_CH_ADC7);
	ADC1_Cmd(ENABLE);
}



void indicateLED(uint16_t result) {
    result /= ADC_prescaler;
    if (result >= 1) {
        FIR_ON;
    } else {
        FIR_OFF;
    }

    if (result >= 2) {
        SEC_ON;
    } else {
        SEC_OFF;
    }

    if (result >= 3) {
        THR_ON;
    } else {
        THR_OFF;
    }

    if (result >= 4) {
        FOUR_ON;
    } else {
        FOUR_OFF;
    }

    if (result >= 5) {
        FTH_ON;
    } else {
        FTH_OFF;
    }

    if (result >= 6) {
        SXTH_ON;
    } else {
        SXTH_OFF;
    }

    if (result >= 7) {
        SCTH_ON;
    } else {
        SCTH_OFF;
    }

    if (result >= 8) {
        EITH_ON;
    } else {
        EITH_OFF;
    }
}
uint16_t moving_average_filter(uint16_t* window, uint16_t newVal) {
    
		static uint8_t i = 0;
		static uint16_t sum = 0;
		if (++i >= WIN_SIZE) i = 0; 
		sum -= window[i];         
		sum += newVal;          
		window[i] = newVal;           
		return (sum / WIN_SIZE);
    
}

  
uint16_t median_filter(uint16_t* window, uint16_t newVal) {

  static uint8_t count = 0;
  window[count] = newVal;
  if ((count < NUM_READ - 1) and (window[count] > window[count + 1])) {
    for (int i = count; i < NUM_READ - 1; i++) {
      if (window[i] > window[i + 1]) {
        uint16_t buff = window[i];
        window[i] = window[i + 1];
        window[i + 1] = buff;
      }
    }
  } else {
    if ((count > 0) and (window[count - 1] > window[count])) {
      for (int i = count; i > 0; i--) {
        if (window[i] < window[i - 1]) {
          uint16_t buff = window[i];
          window[i] = window[i - 1];
          window[i - 1] = buff;
        }
      }
    }
  }
  if (++count >= NUM_READ) count = 0;
  return window[NUM_READ / 2];
}
int main(void)
{
	
	Init_LED();
	Init_ADC();
	uint16_t data[WIN_SIZE] = {0,};
	
	
	while(1){
		
		ADC1_Start();
		while (!ADC_GetFlagStatus(ADC1_FLAG_END_OF_CONVERSION))
		
		indicateLED(median_filter(ADC1_GetResult() & 0x0FFF));
			
			
	}
}
