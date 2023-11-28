#include <syscalls.h>

using namespace cernos;
using namespace cernos::common;
using namespace cernos::hardwarecomm;


SysCallHandler::SysCallHandler(uint8_t interruptNum, InterruptManager* interruptManager)
: InterruptHandler(interruptNum+0x20,interruptManager)
{

}
SysCallHandler::~SysCallHandler(){

}
void printf(char*);
uint32_t SysCallHandler::handleInterrupt(uint32_t esp){
    CPUState* cpu = (CPUState*)esp;

    switch(cpu->eax){//implement all syscalls in here
        case 4:
            printf((char*)cpu->ebx);
            break;
        default:
            break;
    }
    

    return esp;
}