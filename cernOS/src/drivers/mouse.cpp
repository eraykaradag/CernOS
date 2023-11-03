#include <drivers/mouse.h>

using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;



MouseEventHandler::MouseEventHandler(){

}
void MouseEventHandler::OnActivate(){

}
void MouseEventHandler::OnMouseDown(uint8_t button){

}
void MouseEventHandler::OnMouseUp(uint8_t button){

}
void MouseEventHandler::OnMouseMove(int x, int y){

}

MouseDriver::MouseDriver(InterruptManager* interruptManager, MouseEventHandler* handler)
:InterruptHandler(0x2C,interruptManager),
dataPort(0x60),
cmdPort(0x64)
{
	this->handler = handler;
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate(){
	offset = 0;
	buttons = 0;

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

	if((!(status&0x20)) || handler == 0) return esp;

	buffer[offset] = dataPort.Read();
	offset = (offset+1)%3;

	if(offset == 0){
		static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

		if(buffer[1] != 0 || buffer[2] != 0){
			handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));
		}

		
		for(uint8_t i = 0; i < 3; i++){
			if((buffer[0] & (0x01<<i)) != (buttons & (0x01 << i))){
				if(buttons & (0x1<<i)){
					handler->OnMouseUp(i+1);
				}
				else{
					handler->OnMouseDown(i+1);
				}
			}
		}
		buttons = buffer[0];

	}

	return esp;
}