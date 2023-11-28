#ifndef __CERNOS__SYSCALLS_H
#define __CERNOS__SYSCALLS_H
#include <common/types.h>
#include <hardwarecomm/interrupts.h>   
#include <multitasking.h>
namespace cernos{
        class SysCallHandler : public hardwarecomm::InterruptHandler
        {
        public:
            SysCallHandler(cernos::common::uint8_t interruptNum, hardwarecomm::InterruptManager* interruptManager);
            ~SysCallHandler();
            virtual cernos::common::uint32_t handleInterrupt(cernos::common::uint32_t esp);
        };
}
#endif