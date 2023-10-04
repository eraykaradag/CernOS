#include "mouse.h"
MouseDriver::MouseDriver(InterruptManager* interruptManager)
:InterruptHandler(0x2C,interruptManager),
dataPort(0x60),
cmdPort(0x64)
{
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate(){
	offset = 0;
	buttons = 0;
	uint16_t* VideoMemory = (uint16_t*) 0xb8000;
	VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) >> 4) | ((VideoMemory[80*12+40] & 0x0F00) << 4) | ((VideoMemory[80*12+40] & 0x00FF));	

	cmdPort.Write(0xA8); // start sending keyboard interrupts
	cmdPort.Write(0x20); //get curr state

	uint8_t status = (dataPort.Read() | 2);
	cmdPort.Write(0x60); //set state
	dataPort.Write(status);
	
	cmdPort.Write(0XD4);
	dataPort.Write(0xF4);

	dataPort.Read();
}


void printf(char*);

uint32_t MouseDriver::handleInterrupt(uint32_t esp)
{
	uint8_t status = cmdPort.Read();

	if(!(status&0x20)) return esp;

	static int8_t x = 40, y = 12;


	buffer[offset] = dataPort.Read();
	offset = (offset+1)%3;

	if(offset == 0){
		static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

		VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | ((VideoMemory[80*y+x] & 0x00FF));

		x += buffer[1];

		if(x<0) x = 0;
		if(x>= 80) x = 79;

		y -= buffer[2];

		if(y<0) y = 0;
		if(y>= 25) y = 24;
	
		VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | ((VideoMemory[80*y+x] & 0x00FF));

		for(uint8_t i = 0; i < 3; i++){
			if((buffer[0] & (0x01<<i)) != (buttons & (0x01 << i))){
				//...
			}
		}
		buttons = buffer[0];

	}

	return esp;
}