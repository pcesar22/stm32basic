#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_sdram.h"
#include "stm32469i_discovery_lcd.h"
#include "stm32469i_discovery_qspi.h"

#include "lcd_plotter.h"

#define LCD_HEIGHT 480
#define LCD_WIDTH  800

#define PLOT_AREA_X_START 0
#define PLOT_AREA_Y_START 0
#define PLOT_AREA_X_MAX   799
#define PLOT_AREA_Y_MAX   479

#define PLOT_CENTER_LINE (PLOT_AREA_Y_MAX + PLOT_AREA_Y)/2;

#define PLOT_SAMPLE_BUF_SIZE (PLOT_AREA_X_MAX - PLOT_AREA_X_START);

static int plot_y_max_value;
static int plot_y_min_value;

void lcdPlotterInit(int y_max,
                    int y_min,
                    int x_speed)
{
    plot_y_max_value = y_max;
    plot_y_min_value = y_min;
}
