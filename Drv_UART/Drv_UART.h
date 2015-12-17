#ifndef DRV_UART_H_
#define DRV_UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_HW_PORT	(PORTD)
#define UART_HW_DDR		(DDRD)
#define UART_HW_TXD		1
#define UART_HW_RXD		0

#define UART_BAUDRATE 9600UL
#define BAUD	UART_BAUDRATE
#include <util/setbaud.h> // calc UBRRH_VALUE and UBBRL_VALUE for set BAUD

void UART_Init(void);
void UART_WriteByte(uint8_t data);
void UART_WriteArray(uint8_t* array, uint8_t length);
uint8_t UART_ReadByte(void);

#endif /* DRV_UART_H_ */