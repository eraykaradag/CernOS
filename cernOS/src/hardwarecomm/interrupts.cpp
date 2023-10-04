#include "interrupts.h"

void printf(char* str);
void printfHex(uint8_t);

InterruptHandler::InterruptHandler(uint8_t interruptNum, InterruptManager* interruptManager){
	this->interruptNum = interruptNum;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNum] = this;
}

InterruptHandler::~InterruptHandler(){
	if(interruptManager->handlers[interruptNum] == this)
		interruptManager->handlers[interruptNum] = 0;
}

uint32_t InterruptHandler::handleInterrupt(uint32_t esp){
	return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::ActiveInterruptManager = 0;


void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interruptNum, uint16_t codeSegmentSelectorOffset,void (*handler)(), uint8_t DescriptorAccessRights, uint8_t DescriptorType)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;

	interruptDescriptorTable[interruptNum].handlerAddressLow = ((uint32_t) handler) & 0xFFFF;
	interruptDescriptorTable[interruptNum].handlerAddressHigh = (((uint32_t) handler)>>16) & 0xFFFF;
	interruptDescriptorTable[interruptNum].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptNum].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorAccessRights&3) << 5);
	interruptDescriptorTable[interruptNum].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) : picMasterCmd(0x20), picMasterData(0x21),picWorkerCmd(0xA0), picWorkerData(0xA1)
{
	uint16_t codeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	for (uint16_t i = 0; i<256; i++)
	{
		handlers[i] = 0;
		SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
	}
	SetInterruptDescriptorTableEntry(0x20, codeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, codeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x2C, codeSegment, &handleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);


	picMasterCmd.Write(0x11);
	picWorkerCmd.Write(0x11);

	picMasterData.Write(0x20);
	picWorkerData.Write(0x28);

	picMasterData.Write(0x04);
	picWorkerData.Write(0x02);

	picMasterData.Write(0x01);
	picWorkerData.Write(0x01);

	picMasterData.Write(0x00);
	picWorkerData.Write(0x00);

	interruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t) interruptDescriptorTable;

	asm volatile("lidt %0" : : "m" (idt)); 

	printf("idt created\n");

}
InterruptManager::~InterruptManager(){}

void InterruptManager::Activate()
{
	if(ActiveInterruptManager != 0){
		ActiveInterruptManager->Deactivate();
	}
	ActiveInterruptManager = this;
	asm("sti");
}

void InterruptManager::Deactivate()
{
	if(ActiveInterruptManager == this){
		ActiveInterruptManager = 0;
		asm("cli");
	}
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNum, uint32_t esp)
{
	if(ActiveInterruptManager != 0){
		return ActiveInterruptManager->DoHandleInterrupt(interruptNum, esp);
	}
	return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNum, uint32_t esp)
{
	if(handlers[interruptNum] != 0){
		esp = handlers[interruptNum]->handleInterrupt(esp);
	}
	else if(interruptNum != 0x20){
		printf("UNHANDLED INTERRUPT 0x");
		printfHex(interruptNum);
	}
	if(0x20 <= interruptNum && interruptNum < 0x30){
		picMasterCmd.Write(0x20);

		if(0x28 <= interruptNum)
			picWorkerCmd.Write(0x20);
	}
	return esp;
} 