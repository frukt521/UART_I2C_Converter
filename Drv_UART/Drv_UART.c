#include "Drv_UART.h"

void UART_Init(void)
{
		UBRRH = UBRRH_VALUE;	// set bandwidth USART
		UBRRL = UBRRL_VALUE;
		
		// UCSRC:
		// URSEL ��� ������� � UCSRC ���������� ��� ������ ��������� � �������� ���������� ������ ���
		// UMSEL 0 - ����������� � 1 - ���������� �����
		// UPM1:0
		// 0 0 Disabled
		// 0 1 Reserved
		// 1 0 Enabled, Even Parity
		// 1 1 Enabled, Odd Parity
		// USBS - ����� ���������� ����-����� (0 - 1bit, 1 - 2bit)
		
		// UCSZ2(�� UCSRB) UCSZ1 UCSZ0 Character Size
		// 0 0 0 5-bit
		// 0 0 1 6-bit
		// 0 1 0 7-bit
		// 0 1 1 8-bit
		// 1 0 0 Reserved
		// 1 0 1 Reserved
		// 1 1 0 Reserved
		// 1 1 1 9-bit
		// UCPOL
		// 0 Rising XCK Edge Falling XCK Edge
		// 1 Falling XCK Edge Rising XCK Edge
		UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		
		// UCSRB:
		// RXCIE ��������� ���������� �� ������
		// TXCIE �� ��������� ��������
		// UDRIE �� ����������� ������ ��������
		// RXEN ��������� �����
		// TXEN ��������� ��������
		// UCSZ2 ��. UCSRC
		// RXB8 - 9 ���� ��� ������
		// TXB8 - 9 ���� ��� ��������
		UCSRB = (1 << RXEN) | (1 << TXEN);
}

void UART_WriteByte(uint8_t data)
{
	// Wait for empty transmit buffer
	while ( !(UCSRA & (1 << UDRE)) );
	// Put data into buffer, sends the data
	UDR = data;
}

void UART_WriteArray(uint8_t* array, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
		UART_WriteByte(array[i]);
}

uint8_t UART_ReadByte(void)
{
	// Wait for data to be received
	while ( (UCSRA & (1 << RXC)) != (1 << RXC) );
	// Get and return received data from buffer
	return UDR;
}