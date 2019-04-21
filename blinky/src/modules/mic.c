#include "stm32f4xx_hal.h"
#include "mic.h"


#define MIC_RX_TIMEOUT 100
#define MIC_RX_BUFFER_SIZE 2

uint16_t mic_rx_buffer[MIC_RX_BUFFER_SIZE];

I2S_HandleTypeDef hi2s3;

void MX_I2S3_Init(void)
{

    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_RX;
    hi2s3.Init.Standard = I2S_STANDARD_LSB;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_8K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

void MIC_Receive(void)
{
    HAL_I2S_Receive(&hi2s3, mic_rx_buffer, 2, MIC_RX_TIMEOUT);
    printf("Received bytes %04X\r\n", (mic_rx_buffer[0] << 8) | (mic_rx_buffer[1]));
}
