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
                static void handleInterruptRequest0x02();
                static void handleInterruptRequest0x03();
                static void handleInterruptRequest0x04();
                static void handleInterruptRequest0x05();
                static void handleInterruptRequest0x06();
                static void handleInterruptRequest0x07();
                static void handleInterruptRequest0x08();
                static void handleInterruptRequest0x09();
                static void handleInterruptRequest0x0A();
                static void handleInterruptRequest0x0B();
                static void handleInterruptRequest0x0C();
                static void handleInterruptRequest0x0D();
                static void handleInterruptRequest0x0E();
                static void handleInterruptRequest0x0F();
                static void handleInterruptRequest0x31();

                static void handleInterruptRequest0x80();

                /*static void handleException0x00();
                static void handleException0x01();
                static void handleException0x02();
                static void handleException0x03();
                static void handleException0x04();
                static void handleException0x05();
                static void handleException0x06();
                static void handleException0x07();
                static void handleException0x08();
                static void handleException0x09();
                static void handleException0x0A();
                static void handleException0x0B();
                static void handleException0x0C();
                static void handleException0x0D();
                static void handleException0x0E();
                static void handleException0x0F();
                static void handleException0x10();
                static void handleException0x11();
                static void handleException0x12();
                static void handleException0x13();*/


		};
	}
}
	
#endif

