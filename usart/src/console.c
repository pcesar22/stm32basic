/*
  console.c

  Instantiates the usart driver for console usage
*/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

#include "main.h"
#include "console.h"
#include "shell.h"
#include "queue.h"

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      3000
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

#define MAX_NUM_OF_CMDS                   5
#define CLI_CMD_SIZE                      QUEUE_CMD_SIZE

extern shell_cmds console_CommandRegistry;

/* Buffer used for transmission */
uint8_t aTxBuffer[TXBUFFERSIZE];

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

static Queue * console_CommandQueue = NULL;

static const char consolePrefix [] = "\r> ";

static char consoleBuffer[CLI_CMD_SIZE];
static uint32_t consoleIndexLast = 0;
static uint32_t consoleIndexStart = 0;
static bool byteWasReceived = false;

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

static Led_TypeDef m_uartStatusLed;

static void console_ErrorHandler()
{
    BSP_LED_On(m_uartStatusLed);
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
    if (len > TXBUFFERSIZE)
    {
        console_ErrorHandler();
    }
    else
    {
        UartReady = RESET;
        if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)text, len) != HAL_OK)
        {
            console_ErrorHandler();
        }
        while (UartReady != SET);
    }
}

void console_Init(Led_TypeDef uartStatusLed)
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
    else
    {
        if(HAL_UART_Init(&UartHandle) != HAL_OK)
        {
            console_ErrorHandler();
        }
    }

    log_init(write, lock);
    /* log_info("Logger module initialized!"); */

    // Begin command queue
    console_CommandQueue = ConstructQueue(MAX_NUM_OF_CMDS);

    // Begin UART reception
    if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }

}

static void printChar(char c)
{
    UartReady = RESET;
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)&c, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }
    while (UartReady != SET);
}
static void processByte(char byte)
{
    NODE * pN;
    switch(byte)
    {
    case '\b':
        if (consoleIndexLast == 1)
        {
            consoleIndexStart = 0;
            consoleIndexLast = 0;
        }
        else
        {
            consoleIndexLast -= 2;
            consoleIndexStart -= 1;
            printChar(byte);
            printChar(0x20);
            printChar(byte);
            consoleBuffer[consoleIndexLast] = '\0';
        }
        // Can't use "write" for a single byte cause of the '\0' crap
        break;
    case '\r':
        pN = malloc(sizeof(NODE));
        /* Only enqueue the command if there is something in the buffer */
        /* if (consoleIndexLast > 1) */
        /* { */
        consoleBuffer[consoleIndexLast-1] = '\0';
        strcpy(pN->data.cmd, consoleBuffer);
        if (!Enqueue(console_CommandQueue, pN))
        {
            log_warn("Queue is full!");
        };
        consoleIndexLast = 0;
        consoleIndexStart = 0;
        /* } */
        printChar('\n');
        write(consolePrefix);
        memset(consoleBuffer, 0, sizeof(consoleBuffer));
        break;
    default:
        // Echo back the received char
        UartReady = RESET;
        // Can't use "write" for a single byte cause of the '\0' crap
        if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)&byte, 1) != HAL_OK)
        {
            console_ErrorHandler();
        }
        while (UartReady != SET);
        consoleIndexStart += 1;
        break;
    }
}

void console_Process()
{
    NODE *pN;
    int result;

    if (consoleIndexLast != 0)
    {
        if (consoleIndexStart != consoleIndexLast)
        {
            processByte(consoleBuffer[consoleIndexStart]);
        }
    }

    if (!isEmpty(console_CommandQueue))
    {
        pN = Dequeue(console_CommandQueue);
        result = shell_process_cmds(&console_CommandRegistry, pN->data.cmd);
        write(consolePrefix);
        free(pN);
    }
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
    // Receive byte
    uint8_t byte_received = UartHandle->pRxBuffPtr[0];
    consoleBuffer[consoleIndexLast++] = byte_received;

    // Begin next reception
    if (HAL_UART_Receive_DMA(UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
    {
        console_ErrorHandler();
    }

    /* Set transmission flag: trasfer complete*/
    UartReady = SET;
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
