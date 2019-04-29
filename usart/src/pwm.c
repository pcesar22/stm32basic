#include "main.h"
#include "pwm.h"

/* Main Timer Handle */
TIM_HandleTypeDef TimHandle;
TIM_OC_InitTypeDef OcInit;

/* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
uint32_t uwPrescalerValue = 0;

Led_TypeDef m_pwmStatusLed;

static void Error_Handler()
{
    BSP_LED_On(m_pwmStatusLed);
    while (1)
    {
        HAL_Delay(100);
        BSP_LED_Toggle(m_pwmStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_pwmStatusLed);
        HAL_Delay(50);
        BSP_LED_Toggle(m_pwmStatusLed);
        HAL_Delay(50);
        BSP_LED_Toggle(m_pwmStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_pwmStatusLed);
        HAL_Delay(1000);
    }
};

void PWM_Init(Led_TypeDef pwmStatusLed)
{
    /* log_debug("Setting prescaler to %08lX", uwPrescalerValue); */
    m_pwmStatusLed = pwmStatusLed;

    TimHandle.Instance = TIMx;

    // TODO still have to understand how this affects duty cycle exactly
    // 1000000 ---> 13Hz  = 30 * 13/30
    // For 100Hz DC:   X  * 13/30 = 100 -> x = 100 * 30/13 = 230
    // 
    uwPrescalerValue = (uint32_t)((SystemCoreClock/2)/7000000) - 1;

    // GPIO
    GPIO_InitTypeDef  GPIO_InitStruct;


    GPIO_InitStruct.Pin = PWM_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    /* GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; */
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    /* GPIO_InitStruct.Pull = GPIO_PULLUP; */
    GPIO_InitStruct.Alternate = PWM_GPIO_AF;

    PWM_GPIO_CLK_ENABLE();

    HAL_GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStruct);

    /* HAL_GPIO_WritePin(PWM_GPIO_PORT, PWM_GPIO_PIN, GPIO_PIN_SET); */



    /* TimHandle.Init.Prescaler = uwPrescalerValue; */
    TimHandle.Init.Prescaler = uwPrescalerValue;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
    TimHandle.Init.Period = 0xFFFF/2; // TODO
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TimHandle.Init.RepetitionCounter = 0; // TODO

    OcInit.OCMode = TIM_OCMODE_PWM2;
    OcInit.Pulse = 0xFFFF/4;
    OcInit.OCPolarity = TIM_OCPOLARITY_HIGH;
    OcInit.OCFastMode = TIM_OCFAST_ENABLE;

    if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK){
        Error_Handler();
    };
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &OcInit, TIMx_PWM_CHANNEL);
    HAL_TIM_PWM_Init(&TimHandle);
}


void PWM_Start(uint32_t duty_cycle)
{
    if(duty_cycle > 100 || duty_cycle < 0)
    {
        Error_Handler();
        /* log_warn("Duty cycle has to be in 0-100. Range"); */
    }
    OcInit.Pulse = (uint32_t)(TimHandle.Init.Period*(1 - (float)duty_cycle/100.0));
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &OcInit, TIMx_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&TimHandle, TIMx_PWM_CHANNEL);
}

void PWM_Stop()
{
    HAL_TIM_PWM_Stop(&TimHandle, TIMx_PWM_CHANNEL);
}
