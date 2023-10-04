#ifndef __KEYBOARD_H
#define __KEYBOARD_H
	#include "types.h"
	#include "port.h"
	#include "interrupts.h"
	#include "driver.h"

	class KeyboardEventHandler{
	public:
		KeyboardEventHandler();

		virtual void OnKeyDown(char);
		virtual void OnKeyUp(char);
	};

	class KeyboardDriver : public InterruptHandler, public Driver
	{
		Port8bit dataPort;
		Port8bit cmdPort;

		KeyboardEventHandler* handler;
		public:
			KeyboardDriver(InterruptManager* interruptManager, KeyboardEventHandler* handler);
			~KeyboardDriver();
			virtual uint32_t handleInterrupt(uint32_t esp);
			virtual void Activate();
	};
#endif