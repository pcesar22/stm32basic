#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"

#include "usart.h"
#include "gpio.h"

#define TX_BUFFER_SIZE 20
#define BUFFER_SIZE 20
#define USART_TX_TIMEOUT 100
#define MAX_FMT_SIZE  50

UART_HandleTypeDef huart3;

uint8_t uart_tx_buffer[TX_BUFFER_SIZE];
uint8_t uart_rx_buffer[BUFFER_SIZE];

void MX_USART3_UART_Init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /* Put peripheral in reception process */
    HAL_UART_Receive_IT(&huart3, uart_rx_buffer, 1);
}

void UART_Puts(const char * str)
{
    while(*str != '\0')
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)str, 1, USART_TX_TIMEOUT);
        str++;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * uart_handle)
{
    HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);

    uint8_t byte = uart_rx_buffer[0];
    switch(byte)
    {
    case '\r':
        uart_tx_buffer[0] = '\n';
        uart_tx_buffer[1] = '\r';
        HAL_UART_Transmit(&huart3, uart_tx_buffer, 2, USART_TX_TIMEOUT);
        break;
    case '\b':
        uart_tx_buffer[0] = '\b';
        uart_tx_buffer[1] = ' ';
        uart_tx_buffer[2] = '\b';
        HAL_UART_Transmit(&huart3, uart_tx_buffer, 3, USART_TX_TIMEOUT);
        break;
    default:
        HAL_UART_Transmit(&huart3, &byte, 1, USART_TX_TIMEOUT);
        break;
    }

    HAL_UART_Receive_IT(&huart3, uart_rx_buffer, 1);
}

/* Implement printf */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, USART_TX_TIMEOUT);
    return len;
}
