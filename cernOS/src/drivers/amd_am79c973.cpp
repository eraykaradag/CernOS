#include <drivers/amd_am79c973.h>

using namespace cernos;
using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;

void printf(char*);
void printf(uint8_t);
amd_am79c973::amd_am79c973(PCIDeviceDescriptor *dev, InterruptManager* im)
: Driver(), InterruptHandler(dev->interrupt + 0x20,im),
MACAddress0Port(dev->portBase),
MACAddress2Port(dev->portBase+0x02),
MACAddress4Port(dev->portBase+0x04),
registerDataPort(dev->portBase+0x10),
registerAddressPort(dev->portBase+0x12),
resetPort(dev->portBase+0x14),
busControlRegisterDataPort(dev->portBase+0x16)
{
    currSendBuffer = 0;
    currRecvBuffer = 0;

    uint64_t MAC0 = MACAddress0Port.Read()%256;
    uint64_t MAC1 = MACAddress0Port.Read()/256;
    uint64_t MAC2 = MACAddress2Port.Read()%256;
    uint64_t MAC3 = MACAddress2Port.Read()/256;
    uint64_t MAC4 = MACAddress4Port.Read()%256;
    uint64_t MAC5 = MACAddress4Port.Read()/256;

    uint64_t MAC = MAC5 << 40 | MAC4 << 32 | MAC3 << 24 | MAC2 << 16 | MAC1 << 8 | MAC0;
    //32 bit mode
    registerAddressPort.Write(20);
    busControlRegisterDataPort.Write(0x102);

    //stop reset
    registerAddressPort.Write(0);
    registerDataPort.Write(0x04);

    //initblock

    initblock.mode = 0x0000; //promiscuous mode = false
    initblock.reserved1 = 0;
    initblock.numSendBuffers = 3;
    initblock.reserved2 = 0;
    initblock.numRecvBuffers = 3;
    initblock.physicalAddress = MAC;
    initblock.reserved3 = 0;
    initblock.logicalAddress = 0;

    sendBufferDesc = (BufferDescriptor*)((((uint32_t)&sendBufferDescMemory[0]) + 15) & ~((uint32_t)0xF));
    initblock.sendBufferDescAddress = (uint32_t)sendBufferDesc;
    recvBufferDesc = (BufferDescriptor*)((((uint32_t)&recvBufferDescMemory[0]) + 15) & ~((uint32_t)0xF));
    initblock.recvBufferDescAddress = (uint32_t)recvBufferDesc;

    for(uint8_t i = 0; i< 8; i++){
        sendBufferDesc[i].address = (((uint32_t)&sendBuffers[i]) + 15 ) & ~(uint32_t)0xF;
        sendBufferDesc[i].flags = 0x7FF | 0xF000;
        sendBufferDesc[i].flags2 = 0;
        sendBufferDesc[i].avail = 0;

        recvBufferDesc[i].address = (((uint32_t)&recvBuffers[i]) + 15 ) & ~(uint32_t)0xF;
        recvBufferDesc[i].flags = 0xF7FF | 0x80000000;
        recvBufferDesc[i].flags2 = 0;
        recvBufferDesc[i].avail = 0;
    }
    registerAddressPort.Write(1);
    registerDataPort.Write(  (uint32_t)(&initblock) & 0xFFFF );
    registerAddressPort.Write(2);
    registerDataPort.Write(  ((uint32_t)(&initblock) >> 16) & 0xFFFF );
}
amd_am79c973::~amd_am79c973(){

}
void amd_am79c973::Activate(){
    registerAddressPort.Write(0);
    registerDataPort.Write(0x41);

    registerAddressPort.Write(4);
    uint32_t temp = registerDataPort.Read();
    registerAddressPort.Write(4);
    registerDataPort.Write(temp | 0xC00);

    registerAddressPort.Write(0);
    registerDataPort.Write(0x42);
}
int amd_am79c973::Reset(){
    resetPort.Read();
    resetPort.Write(0);
    return 10;
}

uint32_t amd_am79c973::handleInterrupt(uint32_t esp){
    printf("INTERRUPT FROM AMD \n");

    registerAddressPort.Write(0);
    uint32_t temp = registerDataPort.Read();

    if((temp & 0x8000) == 0x8000) printf("AMD am79c973 ERROR \n");
    if((temp & 0x2000) == 0x2000) printf("AMD am79c973 COLLISION ERROR \n");
    if((temp & 0x1000) == 0x1000) printf("AMD am79c973 MISSED FRAME \n");
    if((temp & 0x0800) == 0x0800) printf("AMD am79c973 MEMORY ERROR\n");
    if((temp & 0x0400) == 0x0400) Recieve();
    if((temp & 0x0200) == 0x0200) printf("AMD am79c973 DATA SENT\n");
    //acknowlege
    registerAddressPort.Write(0);
    registerDataPort.Write(temp);

    if((temp & 0x0100) == 0x0100) printf("AMD am79c973 INIT DONE\n");

    return esp;
}

void amd_am79c973::Send(uint8_t* buffer, int count){
    int sendDescriptor = currSendBuffer;
    currRecvBuffer = (currSendBuffer+1)%8;

    if(count > 1518)
        count = 1518;      //fragmentation needed

    for(uint8_t* src = buffer + count -1,*dst = (uint8_t*)(sendBufferDesc[sendDescriptor].address + count -1); src >= buffer; src--,dst--){
        *dst = *src;
    }
    sendBufferDesc[sendDescriptor].avail = 0;
    sendBufferDesc[sendDescriptor].flags2 = 0;
    sendBufferDesc[sendDescriptor].flags = 0x8300f000 | ((uint16_t)((-count)&0xfff));

    registerAddressPort.Write(0);
    registerDataPort.Write(0X48);

}
void amd_am79c973::Recieve(){
    printf("Data has recieved from network card\n");
    
    for(; (recvBufferDesc[currRecvBuffer].flags & 0x80000000) == 0; currRecvBuffer = (currRecvBuffer+1)%8)
    {
        if(!(recvBufferDesc[currRecvBuffer].flags & 0x40000000)&& (recvBufferDesc[currRecvBuffer].flags & 0x03000000) == 0x03000000){
            uint32_t size = recvBufferDesc[currRecvBuffer].flags & 0xFFF;
            if(size > 64) // remove checksum
                size -= 4;

            uint8_t* buffer = (uint8_t*)(recvBufferDesc[currRecvBuffer].address);
            for(int i = 0; i<size; i++){
                printfHex(buffer[i]);
                printf(" ");
            }
        }

        recvBufferDesc[currRecvBuffer].flags2 = 0;
        recvBufferDesc[currRecvBuffer].flags = 0x8000F7FF;
    }
}