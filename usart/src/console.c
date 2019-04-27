/*
  console.c

  Instantiates the usart driver for console usage
*/
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_hal.h"

#include "main.h"
#include "console.h"

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      256
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Buffer used for transmission */
uint8_t aTxBuffer[TXBUFFERSIZE];

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

static Led_TypeDef m_uartStatusLed;

static void console_ErrorHandler()
{
    BSP_LED_On(LED1);
    while (1)
    {
        HAL_Delay(100);
        BSP_LED_Toggle(m_uartStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_uartStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_uartStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_uartStatusLed);
        HAL_Delay(100);
        BSP_LED_Toggle(m_uartStatusLed);
        HAL_Delay(1000);
    }
};

// Interface functions to be used for logging
static void lock(void * data, int lock)
{
    if (lock)
    {
        UartReady = RESET;
    }
    else
    {
        while (UartReady != SET);
    }
}
static void write(char * text)
{
    uint32_t len = strlen(text);
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)text, len) != HAL_OK)
    {
        console_ErrorHandler();
    }
}

int console_Init(Led_TypeDef uartStatusLed)
{
    m_uartStatusLed = uartStatusLed;

    UartHandle.Instance        = USARTx;

    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;

    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        console_ErrorHandler();
    }
    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        console_ErrorHandler();
    }

    log_init(write, lock);
    log_info("Logger module initialized!");

    // Begin UART reception
    if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }

}

void console_Process()
{
    /* log_init("Console started!"); */
}

/**
 * @brief  Tx Transfer completed callback
 * @param  UartHandle: UART handle.
 * @note   This example shows a simple way to report end of DMA Tx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: trasfer complete*/
    UartReady = SET;
}

/**
 * @brief  Rx Transfer completed callback
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report end of DMA Rx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: trasfer complete*/
    UartReady = SET;

    // Receive byte
    uint8_t byte_received = UartHandle->pRxBuffPtr[0];
    if (HAL_UART_Receive_DMA(UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }
    // Echo back the received char
    if (HAL_UART_Transmit_DMA(UartHandle, &byte_received, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }
}

/**
 * @brief  UART error callbacks
 * @param  UartHandle: UART handle
 * @note   This example shows a simple way to report transfer error, and you can
 *         add your own implementation.
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    console_ErrorHandler();
}
