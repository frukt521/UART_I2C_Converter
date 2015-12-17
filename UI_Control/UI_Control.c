#include "UI_Control.h"

const uint8_t UI_Cmd_Prefix[3] = {'U', 'I', '+'};
	
UI_Command_t _cmdControl;

uint8_t UI_CharToHex(uint8_t charH, uint8_t charL)
{
	return ((charH - 0x30) << 4) + charL - 0x30;
}

uint8_t UI_HexToChar(uint8_t hexValue, bool isHigh)
{
	if (isHigh)
		return ((hexValue & 0xF0) >> 4) + 0x30;
	else
		return (hexValue & 0x0F) + 0x30;
	
}

void UI_ClearCmd(void)
{
	_cmdControl.PosInBuf = 0;
	_cmdControl.CmdType = UI_ERROR_CMD;
	_cmdControl.ByteCount = 0;
	_cmdControl.DeviceAddr = 0;
}

void UI_SendErrorCmd(void)
{
	UART_WriteByte('\r');
	UART_WriteArray((uint8_t*)"UI:ERROR", 8);
	UART_WriteByte('\r');
}

void UI_ParceCmd(void)
{
	_cmdControl.CmdType = UI_ERROR_CMD;
	
	//Префикс совпадает?
	for (uint8_t i = 0; i < 3; i++)
	if (_cmdControl.CmdBuf[i] != UI_Cmd_Prefix[i]) return;
	
	//Доп. символы совпадают?
	if ( (_cmdControl.CmdBuf[4] != '=') && (_cmdControl.CmdBuf[7] != ' ') ) return;

	//Определяем тип команды
	switch (_cmdControl.CmdBuf[3])
	{
		case 'R':
			_cmdControl.CmdType = UI_READ;
			break;
		case 'W':
			_cmdControl.CmdType = UI_WRITE;
			break;
		default:
			return;
	}
	
	//Определяем параметры
	_cmdControl.DeviceAddr = UI_CharToHex(_cmdControl.CmdBuf[5], _cmdControl.CmdBuf[6]);
	_cmdControl.ByteCount = UI_CharToHex(_cmdControl.CmdBuf[8], _cmdControl.CmdBuf[9]);
	
	if ( (_cmdControl.DeviceAddr == 0) || (_cmdControl.ByteCount == 0) )
		_cmdControl.CmdType = UI_ERROR_CMD;
}

void UIControl_Init(void)
{
	UI_ClearCmd();
	I2C_Init();
	UART_Init();
}

void UIControl_Start(void)
{
	while (1)
	{
		uint8_t recievedValue = UART_ReadByte();
		//Читаем команду
		if (_cmdControl.PosInBuf < 10)
		{
			_cmdControl.CmdBuf[_cmdControl.PosInBuf] = recievedValue;
			_cmdControl.PosInBuf++;
		}
	
		//Если команда считана
		if (_cmdControl.PosInBuf == 10)
		{
			UI_ParceCmd(); //Парсим тип команды
			//Выполняем соотв. команду или выводим ошибку
			switch (_cmdControl.CmdType)
			{
				case UI_ERROR_CMD:	//Если команда инициализирована как ошибочная
					UI_SendErrorCmd();
					break;
				
				case UI_READ:		//Если команда чтения
					//Считываем данные с I2C в буфер
					I2C_StartCmd();
					I2C_WriteAddress(_cmdControl.DeviceAddr, false);
					//Проверяем наличие устройства на шине
					if (I2C_GetStatus() != 0x40)
					{
						UI_SendErrorCmd();
						break;
					}
					Buffer_SetValue(0, I2C_ReadData(true));
					for (uint8_t i = 1; i < _cmdControl.ByteCount; i++)
						Buffer_SetValue(i, I2C_ReadData(false));
					I2C_StopCmd();
				
					//Формируем ответ по UART
					UART_WriteByte('\r'); //Переход на новую строку
					UART_WriteArray((uint8_t*)"+R:", 3);
					for (uint8_t i = 0; i < _cmdControl.ByteCount; i++)
					{
						UART_WriteByte(UI_HexToChar(Buffer_GetValue(i), true));
						UART_WriteByte(UI_HexToChar(Buffer_GetValue(i), false));
					}
					UART_WriteByte('\r'); //Переход на новую строку
					break;
				
				case UI_WRITE:		//Если команда записи
					//Считываем данные для записи из команды в буфер
					UART_ReadByte(); //Пропускаем пробел
					for (uint8_t i = 0; i < _cmdControl.ByteCount; i++)
					{
						uint8_t valH = UART_ReadByte();
						uint8_t valL = UART_ReadByte();
						Buffer_SetValue(i, UI_CharToHex(valH, valL));
					}
				
					//Запись данных из буфера в I2C устройство
					I2C_StartCmd();
					I2C_WriteAddress(_cmdControl.DeviceAddr, true);
					//Проверяем наличие устройства на шине
					if (I2C_GetStatus() != 0x18)
					{
						UI_SendErrorCmd();
						break;
					}
					for (uint8_t i = 0; i < _cmdControl.ByteCount; i++)
						I2C_WriteData(Buffer_GetValue(i));
					I2C_StopCmd();
				
					//Формируем ответ по UART
					UART_WriteByte('\r'); //Переход на новую строку
					UART_WriteArray((uint8_t*)"+W:OK", 5);
					UART_WriteByte('\r'); //Переход на новую строку
					break;
			}
			//Очищаем структуру команды
			UI_ClearCmd();
		}
	}
}