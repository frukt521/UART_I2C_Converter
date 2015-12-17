#ifndef BUFFER_H_
#define BUFFER_H_

#include <avr/io.h>
#include <stdbool.h>

#define BUFFER_LENGTH	255

void Buffer_SetValue(uint8_t position, uint8_t newValue);
uint8_t Buffer_GetValue(uint8_t position);

#endif /* BUFFER_H_ */