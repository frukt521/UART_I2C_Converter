#include "Buffer.h"

uint8_t _buffer[BUFFER_LENGTH];

uint8_t Buffer_GetValue(uint8_t position)
{
	return _buffer[position];
}

void Buffer_SetValue(uint8_t position, uint8_t newValue)
{
	_buffer[position] = newValue;
}