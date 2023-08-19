#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H
	#include "types.h"
	#include "port.h"
	#include "gdt.h"
	
	class InterruptManager;

	class InterruptHandler
	{
		protected:
			uint8_t interruptNum;
			InterruptManager* interruptManager;

			InterruptHandler(uint8_t interruptNum, InterruptManager* interruptManager);
			~InterruptHandler();
		public:
			virtual uint32_t handleInterrupt(uint32_t esp);

	};
	class InterruptManager
	{
		friend class InterruptHandler;
		protected:
			static InterruptManager* ActiveInterruptManager;
			InterruptHandler* handlers[256];

			struct GateDescriptor{
				uint16_t handlerAddressLow;
				uint16_t gdt_codeSegmentSelector;
				uint8_t reserved;
				uint8_t access;
				uint16_t handlerAddressHigh;
			} __attribute__((packed));

			static GateDescriptor interruptDescriptorTable[256];

			struct interruptDescriptorTablePointer{
				uint16_t size;
				uint32_t base;
			}__attribute__((packed));

			static void SetInterruptDescriptorTableEntry(uint8_t interruptNum, uint16_t codeSegmentSelectorOffset,void (*handler)(), uint8_t DescriptorAccessRights, uint8_t DescriptorType);


			Port8bitSlow picMasterCmd;
			Port8bitSlow picMasterData;
			Port8bitSlow picWorkerCmd;
			Port8bitSlow picWorkerData;
		public:

			InterruptManager(GlobalDescriptorTable* gdt);
			~InterruptManager();

			void Activate();
			void Deactivate();

			static uint32_t handleInterrupt(uint8_t interruptNum, uint32_t esp);
			uint32_t DoHandleInterrupt(uint8_t interruptNum, uint32_t esp);


			static void IgnoreInterruptRequest();
			static void handleInterruptRequest0x00();
			static void handleInterruptRequest0x01();


	};

#endif

