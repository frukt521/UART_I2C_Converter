#include <avr/io.h>

#include "UI_Control/UI_Control.h"

int main(void)
{
	UIControl_Init();
	UIControl_Start();
	
	while(1){} 
}