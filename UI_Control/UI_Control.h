#ifndef UI_CONTROL_H_
#define UI_CONTROL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "../Drv_I2C/Drv_I2C.h"
#include "../Drv_UART/Drv_UART.h"
#include "../Buffer/Buffer.h"

typedef struct
{
	uint8_t CmdBuf[10];
	uint8_t PosInBuf;
	uint8_t CmdType;
	uint8_t DeviceAddr;
	uint8_t ByteCount;
} UI_Command_t;

typedef enum
{
	UI_READ,
	UI_WRITE,
	UI_ERROR_CMD,
	UI_R_ERROR,
	UI_W_ERROR
} UI_Command_e;

void UIControl_Init(void);
void UIControl_Start(void);

#endif /* UI_CONTROL_H_ */