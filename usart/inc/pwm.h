#ifndef __PWM_H__
#define __PWM_H__

#include "main.h"

#define TIMx                       TIM3
#define TIMx_CLK_ENABLE()          __HAL_RCC_TIM3_CLK_ENABLE()
#define TIMx_CLK_DISABLE()         __HAL_RCC_TIM3_CLK_DISABLE()
#define TIMx_PWM_CHANNEL           TIM_CHANNEL_2

#define PWM_GPIO_PIN               GPIO_PIN_5
#define PWM_GPIO_PORT              GPIOB
#define PWM_GPIO_AF                GPIO_AF2_TIM3
#define PWM_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

void PWM_Init(Led_TypeDef pwmStatusLed);

void PWM_Start(uint32_t duty_cycle);

void PWM_Stop();

#endif
