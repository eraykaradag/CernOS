#ifndef __CERNOS__HARDWARECOMM__INTERRUPTS_H
#define __CERNOS__HARDWARECOMM__INTERRUPTS_H
#include <common/types.h>
#include <hardwarecomm/port.h>
#include <gdt.h>
#include <multitasking.h>

namespace cernos{
	namespace hardwarecomm{
		class InterruptManager;

		class InterruptHandler
		{
			protected:
				cernos::common::uint8_t interruptNum;
				InterruptManager* interruptManager;

				InterruptHandler(cernos::common::uint8_t interruptNum, InterruptManager* interruptManager);
				~InterruptHandler();
			public:
				virtual cernos::common::uint32_t handleInterrupt(cernos::common::uint32_t esp);

		};
		class InterruptManager
		{
			friend class InterruptHandler;
			protected:
				static InterruptManager* ActiveInterruptManager;
				InterruptHandler* handlers[256];
				TaskManager* taskManager;

				struct GateDescriptor{
					cernos::common::uint16_t handlerAddressLow;
					cernos::common::uint16_t gdt_codeSegmentSelector;
					cernos::common::uint8_t reserved;
					cernos::common::uint8_t access;
					cernos::common::uint16_t handlerAddressHigh;
				} __attribute__((packed));

				static GateDescriptor interruptDescriptorTable[256];

				struct interruptDescriptorTablePointer{
					cernos::common::uint16_t size;
					cernos::common::uint32_t base;
				}__attribute__((packed));

				static void SetInterruptDescriptorTableEntry(cernos::common::uint8_t interruptNum, cernos::common::uint16_t codeSegmentSelectorOffset,void (*handler)(), cernos::common::uint8_t DescriptorAccessRights, cernos::common::uint8_t DescriptorType);


				cernos::hardwarecomm::Port8bitSlow picMasterCmd;
				cernos::hardwarecomm::Port8bitSlow picMasterData;
				cernos::hardwarecomm::Port8bitSlow picWorkerCmd;
				cernos::hardwarecomm::Port8bitSlow picWorkerData;
			public:

				InterruptManager(cernos::GlobalDescriptorTable* gdt, cernos::TaskManager* taskManager);
				~InterruptManager();

				void Activate();
				void Deactivate();

				static cernos::common::uint32_t handleInterrupt(cernos::common::uint8_t interruptNum, cernos::common::uint32_t esp);
				cernos::common::uint32_t DoHandleInterrupt(cernos::common::uint8_t interruptNum, cernos::common::uint32_t esp);


				static void IgnoreInterruptRequest();
				static void handleInterruptRequest0x00();
				static void handleInterruptRequest0x01();
				static void handleInterruptRequest0x0C();


		};
	}
}
	
#endif

