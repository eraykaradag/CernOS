#include <hardwarecomm/port.h>

using namespace cernos::common;
using namespace cernos::hardwarecomm;

Port::Port(uint16_t portNum){
	this->portNum = portNum;
}
Port::~Port(){
}

Port8bit::Port8bit(uint16_t portNum): Port(portNum)
{
}
Port8bit::~Port8bit(){
}

void Port8bit::Write(uint8_t data){

	__asm__ volatile("outb %0, %1" : : "a" (data),"Nd" (portNum));
}

uint8_t Port8bit::Read(){
	uint8_t res;

	__asm__ volatile("inb %1, %0" : "=a" (res) :  "Nd" (portNum));

	return res;
}

Port8bitSlow::Port8bitSlow(uint16_t portNum): Port8bit(portNum)
{
}
Port8bitSlow::~Port8bitSlow(){
}

void Port8bitSlow::Write(uint8_t data){

	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f \n 1:" : : "a" (data),"Nd" (portNum));
}


Port16bit::Port16bit(uint16_t portNum): Port(portNum)
{
}
Port16bit::~Port16bit(){
}

void Port16bit::Write(uint16_t data){

	__asm__ volatile("outw %0, %1" : : "a" (data),"Nd" (portNum));
}

uint16_t Port16bit::Read(){
	uint16_t res;

	__asm__ volatile("inw %1, %0" : "=a" (res) :  "Nd" (portNum));

	return res;
}

Port32bit::Port32bit(uint16_t portNum): Port(portNum)
{
}
Port32bit::~Port32bit(){
}

void Port32bit::Write(uint32_t data){

	__asm__ volatile("outl %0, %1" : : "a" (data),"Nd" (portNum));
}

uint32_t Port32bit::Read(){
	uint32_t res;

	__asm__ volatile("inl %1, %0" : "=a" (res) :  "Nd" (portNum));

	return res;
}