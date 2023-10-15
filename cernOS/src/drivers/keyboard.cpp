#include <drivers/keyboard.h>

using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;

KeyboardEventHandler::KeyboardEventHandler(){
}

void KeyboardEventHandler::OnKeyDown(char){
}

void KeyboardEventHandler::OnKeyUp(char){
}

KeyboardDriver::KeyboardDriver(InterruptManager* interruptManager, KeyboardEventHandler* handler)
:InterruptHandler(0x21,interruptManager),
dataPort(0x60),
cmdPort(0x64)
{
	this->handler = handler;
}

KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::Activate(){
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

void printf(char*);
void printfHex(uint8_t);
void backspace();

uint32_t KeyboardDriver::handleInterrupt(uint32_t esp)
{

	uint8_t key = dataPort.Read();
	static bool caps = false;
	static bool shift = false;

	if(handler == 0){
		return esp;
	}

	if(key<80 || key == 0xAA || key == 0xB6){
		switch(key)
		{	
			case 0xFA: break;
			case 0x45: case 0xC5: break;
			case 0x02: handler->OnKeyDown('1'); break;
            case 0x03: handler->OnKeyDown('2'); break;
            case 0x04: handler->OnKeyDown('3'); break;
            case 0x05: handler->OnKeyDown('4'); break;
            case 0x06: handler->OnKeyDown('5'); break;
            case 0x07: handler->OnKeyDown('6'); break;
            case 0x08: handler->OnKeyDown('7'); break;
            case 0x09: handler->OnKeyDown('8'); break;
            case 0x0A: handler->OnKeyDown('9'); break;
            case 0x0B: handler->OnKeyDown('0'); break;	
            case 0x0C: handler->OnKeyDown('*'); break;
            case 0x0D: handler->OnKeyDown('-'); break;	

            case 0x10: if(shift||caps) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); break;
            case 0x11: if(shift||caps) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
            case 0x12: if(shift||caps) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
            case 0x13: if(shift||caps) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
            case 0x14: if(shift||caps) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
            case 0x15: if(shift||caps) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
            case 0x16: if(shift||caps) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
            case 0x17: if(shift||caps) handler->OnKeyDown('I'); else handler->OnKeyDown('ı'); break;
            case 0x18: if(shift||caps) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
            case 0x19: if(shift||caps) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); break;
			case 0x1A: if(shift||caps) handler->OnKeyDown('Ğ'); else handler->OnKeyDown('\u011f'); break;
			case 0x1B: if(shift||caps) handler->OnKeyDown('Ü'); else handler->OnKeyDown('ü'); break;            	

            case 0x1E: if(shift||caps) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
            case 0x1F: if(shift||caps) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
            case 0x20: if(shift||caps) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
            case 0x21: if(shift||caps) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); break;
            case 0x22: if(shift||caps) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); break;
            case 0x23: if(shift||caps) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
            case 0x24: if(shift||caps) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); break;
            case 0x25: if(shift||caps) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
            case 0x26: if(shift||caps) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
			case 0x27: if(shift||caps) handler->OnKeyDown('Ş'); else handler->OnKeyDown('ş'); break;            	
			case 0x28: if(shift||caps) handler->OnKeyDown('İ'); else handler->OnKeyDown('i'); break;
			case 0x2B: if(shift) handler->OnKeyDown(';'); else handler->OnKeyDown(','); break;      	

            case 0x2C: if(shift||caps) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
            case 0x2D: if(shift||caps) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
            case 0x2E: if(shift||caps) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); break;
            case 0x2F: if(shift||caps) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
            case 0x30: if(shift||caps) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); break;
            case 0x31: if(shift||caps) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
            case 0x32: if(shift||caps) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); break;
			case 0x33: if(shift||caps) handler->OnKeyDown('Ö'); else handler->OnKeyDown('ö'); break;            	
			case 0x34: if(shift||caps) handler->OnKeyDown('Ç'); else handler->OnKeyDown('ç'); break;            	
            case 0x35: if(shift) handler->OnKeyDown(':'); else handler->OnKeyDown('.'); break;

            case 0x1C: handler->OnKeyDown('\n'); break;
            case 0x39: handler->OnKeyDown(' '); break;
           	case 0x0E: backspace(); break;

            case 0x3A: caps = !caps; break; //caps
            case 0x2A: case 0x36: shift = true; break; //shift pressed
            case 0xAA: case 0xB6: shift = false; break; //shift released
			default:
				printf("KEYBOARD 0x");
				printfHex(key);
				break;	
		}		

	}
	return esp;
}