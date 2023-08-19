#include "keyboard.h"
KeyboardDriver::KeyboardDriver(InterruptManager* interruptManager)
:InterruptHandler(0x21,interruptManager),
dataPort(0x60),
cmdPort(0x64)
{
	while(cmdPort.Read() &	0x1){
		dataPort.Read();
	}
	cmdPort.Write(0xAE); // start sending keyboard interrupts
	cmdPort.Write(0x20); //get curr state

	uint8_t status = (dataPort.Read() | 1) & ~0x10;
	cmdPort.Write(0x60); //set state
	dataPort.Write(status);
	dataPort.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{

	uint8_t key = dataPort.Read();

	/*if(key<0x80){
		switch(key)
		{	
			case 0xFA: break;
			case 0x45: case 0xC5: break;
			default:*/
				char* foo = "KEYBOARD 0X00 ";
				char* hex = "0123456789ABCDEF";
				foo[11] = hex[(key >> 4) & 0x0F];
				foo[12] = hex[key & 0x0F];
				printf(foo);			
		//}		
	//}	
	return esp;
}