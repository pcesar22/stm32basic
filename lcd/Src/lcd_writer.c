#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_sdram.h"
#include "stm32469i_discovery_lcd.h"
#include "stm32469i_discovery_qspi.h"

#include "lcd_writer.h"

#define LCD_HEIGHT 480
#define LCD_WIDTH  800

#define DEFAULT_FONT Font24
#define FONT_WIDTH   17
#define FONT_HEIGHT  24

static uint32_t current_line = 0;
static uint32_t current_col = 0;

static uint32_t current_line_start = 0;
static uint32_t current_col_start = 0;
const static uint32_t current_line_max = (uint32_t)(LCD_HEIGHT/FONT_HEIGHT);
const static uint32_t current_col_max = (uint32_t)(LCD_WIDTH/FONT_WIDTH);

extern DSI_HandleTypeDef * hdsi_eval;

static void clear_all_lines()
{
    for (uint32_t i = 0; i < current_line_max; i++)
    {
        BSP_LCD_ClearStringLine(i);
    }
}


void lcd_write(char * text)
{
    uint32_t len = strlen(text);

    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

    if (current_line == current_line_max - 1)
    {
        current_line = 0;
        clear_all_lines();
    }

    else
    {
        current_line += 1;
    }

    BSP_LCD_DisplayStringAtLine(current_line, (uint8_t *)text);
    HAL_DSI_Refresh(&hdsi_eval);
}
