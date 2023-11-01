#ifndef __CERNOS__DRIVERS__MOUSE_H
#define __CERNOS__DRIVERS__MOUSE_H
#include <common/types.h>
#include <hardwarecomm/port.h>
#include <hardwarecomm/interrupts.h>
#include <drivers/driver.h>
namespace cernos{
	namespace drivers{
		class MouseEventHandler{
			cernos::common::uint8_t x;
			cernos::common::uint8_t y;
			public:
				MouseEventHandler();
				virtual void OnActivate();
				virtual void OnMouseDown(cernos::common::uint8_t button);
				virtual void OnMouseUp(cernos::common::uint8_t button);
				virtual void OnMouseMove(int x, int y);
		};

		class MouseDriver : public cernos::hardwarecomm::InterruptHandler, public Driver
		{
			cernos::hardwarecomm::Port8bit dataPort;
			cernos::hardwarecomm::Port8bit cmdPort;

			cernos::common::uint8_t buffer[3];
			cernos::common::uint8_t offset;
			cernos::common::uint8_t buttons;
			
			MouseEventHandler* handler;
			public:
				MouseDriver(cernos::hardwarecomm::InterruptManager* interruptManager,MouseEventHandler* handler);
				~MouseDriver();
				virtual cernos::common::uint32_t handleInterrupt(cernos::common::uint32_t esp);
				virtual void Activate();
		};
	}
}
#endif