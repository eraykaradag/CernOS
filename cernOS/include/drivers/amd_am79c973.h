#ifndef __CERNOS__DRIVERS__AMD_AM79C973_H
#define __CERNOS__DRIVERS__AMD_AM79C973_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/pci.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/port.h>
#include <memorymanagement.h>
namespace cernos{
    namespace drivers{
        class amd_am79c973: public Driver, public hardwarecomm::InterruptHandler{
            
            struct InitializationBlock{
                common::uint16_t mode;
                unsigned reserved1 : 4;
                unsigned numSendBuffers : 4;
                unsigned reserved2 : 4;
                unsigned numRecvBuffers : 4;
                common::uint64_t physicalAddress : 48;
                common::uint16_t reserved3;
                common::uint64_t logicalAddress;
                common::uint32_t recvBufferDescAddress;
                common::uint32_t sendBufferDescAddress;
            } __attribute__((packed));

            struct BufferDescriptor{
                common::uint32_t address;
                common::uint32_t flags;
                common::uint32_t flags2;
                common::uint32_t avail;
            } __attribute__((packed));


            hardwarecomm::Port16bit MACAddress0Port;
            hardwarecomm::Port16bit MACAddress2Port;
            hardwarecomm::Port16bit MACAddress4Port;
            hardwarecomm::Port16bit registerDataPort;
            hardwarecomm::Port16bit registerAddressPort;
            hardwarecomm::Port16bit resetPort;
            hardwarecomm::Port16bit busControlRegisterDataPort;

            InitializationBlock initblock;

            BufferDescriptor* sendBufferDesc;
            common::uint8_t sendBufferDescMemory[2048+15];
            common::uint8_t sendBuffers[2*1024+15][8];
            common::uint8_t currSendBuffer;

            BufferDescriptor* recvBufferDesc;
            common::uint8_t recvBufferDescMemory[2048+15];
            common::uint8_t recvBuffers[2*1024+15][8];
            common::uint8_t currRecvBuffer;

            public:
                amd_am79c973(cernos::hardwarecomm::PCIDeviceDescriptor* dev, cernos::hardwarecomm::InterruptManager* im);
                ~amd_am79c973();

                void Activate();
				int Reset();
                common::uint32_t handleInterrupt(common::uint32_t esp);

                void Send(common::uint8_t* buffer, int count);
                void Recieve();
        };
    }
}


#endif