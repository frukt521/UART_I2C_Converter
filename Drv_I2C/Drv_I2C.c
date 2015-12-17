#include "Drv_I2C.h"

void I2C_Init()
{
	//TWI Status Register
	TWSR &= ~(1 << TWPS1) & ~(1 << TWPS0);
	/*
	Prescaler bits
	TWPS1	TWPS0	Prescaler
	0		0		1
	0		1		4
	1		0		16
	1		1		64
	
	TWS7..TWS3	- TWI Status
	*/
	
	//TWI BitRate Register
	TWBR = ( (F_CPU/F_I2C)-16 ) / (2 * 4^0);	//Value = 72
	
	//TWI Control Register
	/*
	TWEN	- TWI Enable
	TWIE	- TWI Interrupt Enable
	
	TWWC	- TWI Write Collision Flag
	TWINT	- TWI Interrupt Flag
		
	TWSTA	- TWI Start Bit
	TWSTO	- TWI Stop Bit
	TWEA	- TWI Enable Acknowledge Bit
	*/
	//TWCR |= (1 << TWEN);
}

void I2C_StartCmd()
{
	//Clear Interrupt flag and send start command
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	_I2C_WAIT_FINISH();
}

void I2C_StopCmd()
{
	//Clear Interrupt flag and send stop command
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2C_WriteAddress(uint8_t deviceAddr, bool isWrite)
{
	if (isWrite)
		TWDR = (deviceAddr << 1);
	else
		TWDR = (deviceAddr << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	_I2C_WAIT_FINISH();
}

void I2C_WriteData(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	_I2C_WAIT_FINISH();
}

uint8_t I2C_ReadData(bool ackEnable)
{
	if (ackEnable)
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	else
		TWCR = (1<<TWINT)|(1<<TWEN);
	_I2C_WAIT_FINISH();
	return TWDR;
}

uint8_t I2C_GetStatus(void)
{
	return (TWSR & 0b11111000);
}