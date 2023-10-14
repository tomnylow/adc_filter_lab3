#include "MDR32Fx.h"                    // Device header
#include "MDR32F9Qx_config.h"           // Keil::Device:Startup
#include "MDR32F9Qx_adc.h"              // Keil::Drivers:ADC
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT
#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK


#define FIR_ON PORT_SetBits (MDR_PORTA, PORT_Pin_3)
#define FIR_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_3)

#define SEC_ON PORT_SetBits (MDR_PORTA, PORT_Pin_7)
#define SEC_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_7)

#define THR_ON PORT_SetBits (MDR_PORTA, PORT_Pin_4)
#define THR_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_4)

#define FOUR_ON PORT_SetBits (MDR_PORTA, PORT_Pin_5)
#define FOUR_OFF PORT_ResetBits (MDR_PORTA, PORT_Pin_5)

#define FTH_ON PORT_SetBits (MDR_PORTB, PORT_Pin_0)
#define FTH_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_0)

#define SXTH_ON PORT_SetBits (MDR_PORTB, PORT_Pin_2)
#define SXTH_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_2)

#define SCTH_ON PORT_SetBits (MDR_PORTB, PORT_Pin_1)
#define SCTH_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_1)

#define EITH_ON PORT_SetBits (MDR_PORTB, PORT_Pin_5)
#define EITH_OFF PORT_ResetBits (MDR_PORTB, PORT_Pin_5)

#define ADC_prescaler 227

void Init_PortA ()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTA, ENABLE);
	PORT_InitTypeDef PORTA_InitStruct;
	PORTA_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORTA_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORTA_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORTA_InitStruct.PORT_OE = PORT_OE_OUT;
	PORTA_InitStruct.PORT_Pin = PORT_Pin_All;
	PORTA_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PORTA_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTA, &PORTA_InitStruct);
}

void Init_PortB ()
{
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTB, ENABLE);
	PORT_InitTypeDef PORTB_InitStruct;
	PORTB_InitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORTB_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
	PORTB_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
	PORTB_InitStruct.PORT_OE = PORT_OE_OUT;
	PORTB_InitStruct.PORT_Pin = PORT_Pin_All;
	PORTB_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PORTB_InitStruct.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (MDR_PORTB, &PORTB_InitStruct);
}
/*void initADC1() {
    // ????????? ???????????? ????? ADC1
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_ADC;

    // ????? ???????? ADC1
    ADC_DeInit();

    // ????????? ?????? ADC1 ??? ?????????
    ADCx_InitTypeDef ADC1_InitStruct;
    
    
    // ??????? ??????????
    ADC1_InitStruct.ADC_ClockSource = ADC_CLOCK_SOURCE_CPU;
    ADC1_InitStruct.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
    ADC1_InitStruct.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
    ADC1_InitStruct.ADC_ChannelNumber = ADC_CH_ADC1;
    ADC1_InitStruct.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
    ADC1_InitStruct.ADC_LowLevel = 0;
    ADC1_InitStruct.ADC_HighLevel = 0;
    ADC1_InitStruct.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
    ADC1_InitStruct.ADC_Prescaler = ADC_CLK_div_32768;
    ADC_Init(&ADC1_InitStruct);

}*/
void setLED(int result) {
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

int main(void) {

    Init_PortA ();
		Init_PortB ();
		//Init_ADC ();
	
    while (1) {

        ADC1_Start();
        
        while (ADC1_GetFlagStatus(ADC1_FLAG_END_OF_CONVERSION) == RESET);
        int result = ADC1_GetResult();
				
        setLED(result);
    }
		
return 0;
}