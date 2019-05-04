/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdarg.h>

#include "main.h"

#include "sysinit.h"
#include "console.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TRANSMITTER_BOARD

__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */


ADC_HandleTypeDef AdcHandle;

__IO uint16_t uhADCxConvertedValue = 0;

static bool flag_received = false;

int main(void)
{

    // Initialize clock and main peripherals
    sysinit_InitAll();

    // Logger is also initialized within the console module
    console_Init(LED1);
    PWM_Init(LED2);


    // ADC STuff.... pin A0 = PB1 = ADC12_IN9
    ADC_ChannelConfTypeDef sConfig;
    AdcHandle.Instance = ADC2;
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.ScanConvMode = DISABLE;
    AdcHandle.Init.ContinuousConvMode = ENABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC3;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion = 1;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection = DISABLE;

    HAL_ADC_Init(&AdcHandle);

    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    sConfig.Offset       = 0;

    log_info("Main program started.");
    log_debug("System core clock: %d", SystemCoreClock);

    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

    HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, 1);

    /* while(flag_received == false); */
    /* log_debug("Received flag! Value: %04X", uhADCxConvertedValue); */

    while (1)
    {
        console_Process();
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    /* Turn LED1 on: Transfer process is correct */
    /* HAL_ADC_Stop_DMA(AdcHandle); */
    /* log_debug("readout: %04X", uhADCxConvertedValue); */
    /* HAL_ADC_Start_DMA(AdcHandle, (uint32_t*)&uhADCxConvertedValue, 1); */
    flag_received = true;
}


/**
 * @brief EXTI line detection callbacks
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == USER_BUTTON_PIN)
    {
        UserButtonStatus = 1;
    }
}
