#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

/* #define TIMx                       TIM3 */
/* #define TIMx_CLK_ENABLE()          __HAL_RCC_TIM3_CLK_ENABLE() */
/* #define TIMx_CLK_DISABLE()         __HAL_RCC_TIM3_CLK_DISABLE() */
/* #define TIMx_ADC_CHANNEL           TIM_CHANNEL_2 */

/* #define ADC_GPIO_PIN               GPIO_PIN_5 */
/* #define ADC_GPIO_PORT              GPIOB */
/* #define ADC_GPIO_AF                GPIO_AF1 */
/* #define ADC_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() */

void ADC_Init(Led_TypeDef adcStatusLed);

void ADC_Start();

void ADC_Stop();

#endif
