#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "stm32469i_discovery.h"

void console_Init(void);
void console_Process(void);

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#endif
