#ifndef __CERNOS__DRIVERS__KEYBOARD_H
#define __CERNOS__DRIVERS__KEYBOARD_H
#include <common/types.h>
#include <hardwarecomm/port.h>
#include <hardwarecomm/interrupts.h>
#include <drivers/driver.h>
namespace cernos{
	namespace drivers{
		

		class KeyboardEventHandler{
		public:
			KeyboardEventHandler();

			virtual void OnKeyDown(char);
			virtual void OnKeyUp(char);
		};

		class KeyboardDriver : public cernos::hardwarecomm::InterruptHandler, public Driver
		{
			cernos::hardwarecomm::Port8bit dataPort;
			cernos::hardwarecomm::Port8bit cmdPort;

			KeyboardEventHandler* handler;
			public:
				KeyboardDriver(cernos::hardwarecomm::InterruptManager* interruptManager, KeyboardEventHandler* handler);
				~KeyboardDriver();
				virtual cernos::common::uint32_t handleInterrupt(cernos::common::uint32_t esp);
				virtual void Activate();
		};
	}
}
#endif