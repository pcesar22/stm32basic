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

int main(void)
{

    // Initialize clock and main peripherals
    sysinit_InitAll();

    // Logger is also initialized within the console module
    console_Init(LED1);
    PWM_Init(LED2);

    /* Configure User push-button in Interrupt mode */
    /* BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI); */

    /* log_info("Pressione o botao para acalmar a LOML"); */

    /* while(UserButtonStatus == 0) */
    /* { */
    /*     /\* Toggle LED2*\/ */
    /*     BSP_LED_Toggle(LED2); */
    /*     HAL_Delay(100); */
    /* } */
    /* UserButtonStatus = 1; */
    /* BSP_LED_Off(LED2); */

    log_info("Main program started.");
    log_debug("System core clock: %d", SystemCoreClock);


    // Start recurring timer with interrupt

    while (1)
    {
        console_Process();
        /* HAL_Delay(400); */
    }
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
