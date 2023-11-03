#include <common/types.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <gdt.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/pci.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <common/img.h>
#include <gui/desktop.h>
#include <gui/window.h>

#define GRAPHICSMODE

using namespace cernos;
using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;
using namespace cernos::gui;

static uint8_t x = 0 , y = 0;
static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

void backspace(){
	if(x>0){
		x--;
		VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
	}
	else{
		x = 0;
	}
}

void clearScreen(){
	for (y = 0; y < 25; ++y)
	{
		for (x = 0; x < 80; ++x)
		{
			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
		}
	}
	x = 0; y = 0;
}

void printf(char* str){
	for (int i = 0; str[i] != '\0'; i++){

		switch(str[i]){
			case '\n':
				y++;
				x = 0;
				break;
			default:
				VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
				x++;
		}
		if(x>= 80){
			y++;
			x = 0;
		}
		if(y>= 25){
			clearScreen();
		}
	}
}
void printfHex(uint8_t key){
	char* foo = "00";
	char* hex = "0123456789ABCDEF";
	foo[0] = hex[(key >> 4) & 0x0F];
	foo[1] = hex[key & 0x0F];
	printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
public:
	void OnKeyDown(char c){
		char* foo = " ";
		foo[0] = c;
		printf(foo);
	}
};
class MouseToConsole : public MouseEventHandler{
	uint8_t x,y;
	public:
		MouseToConsole(){
			uint16_t* VideoMemory = (uint16_t*) 0xb8000;
			x = 40;
			y = 12;
			VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) >> 4) | ((VideoMemory[80*12+40] & 0x0F00) << 4) | ((VideoMemory[80*12+40] & 0x00FF));	

		}
		void OnMouseMove(int xoffset, int yoffset){
			
			VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | ((VideoMemory[80*y+x] & 0x00FF));

				x += xoffset;

				if(x<0) x = 0;
				if(x>= 80) x = 79;

				y -= yoffset;

				if(y<0) y = 0;
				if(y>= 25) y = 24;
			
				VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | ((VideoMemory[80*y+x] & 0x00FF));
	}
};


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for(constructor* i = &start_ctors; i != &end_ctors; i++){
		(*i)();
	}
}


extern "C" void kernelMain(const void* multiboot_structure, uint32_t magicnum){
	

	/* printf("=======++++++++***##%%%%%%%%%%**##****************\n");
	printf("======++*****##%%%%%%%%%@@@@%%***#****************\n");
	printf("====+***#%%@@@@@@%@@%*#%%%@@@@%%*##**************#\n");
	printf("==+*##%###%%%%%%@@%@*#%%%@@@@@@@%%##*************#\n");
	printf("---==++***######%%%#+*#%%%%%%%@@@%%%%#***********#\n");
	printf("-------==+*#**%%##**+########%%%%%@@%%##*********#\n");
	printf("========+**###%#+++*###*#*#%#%%%%@%@%%###********#\n");
	printf("######*+**#%##%%++*%%###*%%##%@%@%%%%%###****#####\n");
	printf("@@@@%%%%%%#%###*=+*++*##%%%%%@@@@@@%@%%###########\n");	
	printf("@@%%%%%%@@%#*+++=======+*###%%%@@@@%@%%###########\n");
	printf("@@@%%%@@@@%%##*+====--=++*#%#%%%@@%%%###########*#\n");
	printf("@@@@@%%@@@@%%@#*+===++++*#%%#%%@%@@@@%**#%%%######\n");
	printf("@@@@%%#%%@@@@@@%#*+++++***####%%%%@%@%**%%%%######\n");
	printf("##%@%%%#%@@@@@@@%++===+++**###%%%@@@@@%%%#%@%#####\n");
	printf("##%%%#%%%@@@@@@@@++=====+*#*##%%#%@@@@%%%#########\n");
	printf("#%@%#*##*%@@@@@@%*+=====+*****%##%%@@@@%%%########\n");
	printf("=+*##%%%#@%%@@@%%#+=====+++*###%%@@@@%%@@@@%%#####\n");
	printf("-=+#%@@@@%##%%#%@%*+====++++*%%%@@%%*#%@@@%**+=+*#\n");
	printf("===+**#@@@####%@@@#+=======+#%%@@%###%%%%%++=====+\n"); */
	//printf("======+#*#%%%%@@@@*========+*##%%%##**+++===-----+\n");

	GlobalDescriptorTable gdt;
	InterruptManager im(&gdt);
	
	Desktop desktop(320,200,0x00,0x00,0xA8);

	DriverManager dm(0);
#ifndef GRAPHICSMODE
	PrintfKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard(&im, &kbhandler);
#endif
#ifdef GRAPHICSMODE
	KeyboardDriver keyboard(&im, &desktop);
#endif
	dm.AddDriver(&keyboard);

#ifndef GRAPHICSMODE
	MouseToConsole mousehandler;
	MouseDriver mouse(&im,&mousehandler);
#endif
#ifdef GRAPHICSMODE
	MouseDriver mouse(&im,&desktop);
#endif
	dm.AddDriver(&mouse);

	PCIController pci;
	pci.SelectDrivers(&dm, &im);
#ifdef GRAPHICSMODE
	VideoGraphicsArray vga;
#endif

	dm.ActivateAll();
#ifdef GRAPHICSMODE
	vga.SetMode(320,200,8);
	Window win1(&desktop,10,10,20,20,0XA8,0X00,0X00);
	desktop.AddChild(&win1);
	Window win2(&desktop,40,15,30,30,0x00,0XA8,0X00);
	desktop.AddChild(&win2);
#endif
	im.Activate();


	while(1){
		#ifdef GRAPHICSMODE
		desktop.Draw(&vga);
		#endif
	}
		
			
}