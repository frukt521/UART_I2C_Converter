#ifndef DRV_I2C_H_
#define DRV_I2C_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_I2C	50000UL

#define _I2C_WAIT_FINISH()		do{while(!(TWCR & (1 << TWINT)));} while(0)

void I2C_Init();
void I2C_StartCmd();
void I2C_StopCmd();
void I2C_WriteAddress(uint8_t deviceAddr, bool isWrite);
void I2C_WriteData(uint8_t data);
uint8_t I2C_ReadData(bool ackEnable);
uint8_t I2C_GetStatus(void);



#endif /* DRV_I2C_H_ */