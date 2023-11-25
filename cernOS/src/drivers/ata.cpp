#include <drivers/ata.h>

using namespace cernos;
using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;

void printf(char*);

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(common::uint16_t portBase, bool master)
: dataPort(portBase),
errorPort(portBase+1),
sectorCountPort(portBase+2),
lbaLowPort(portBase+3),
lbaMidPort(portBase+4),
lbaHighPort(portBase+5),
devicePort(portBase+6),
cmdPort(portBase+7),
controlPort(portBase+0x206)
{
    bytesPerSector = 512;
    this->master = master;
}
AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment(){

}
void AdvancedTechnologyAttachment::Identify(){
    devicePort.Write(master ? 0xA0 : 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);
    uint8_t status = cmdPort.Read();
    if(status == 0XFF) return;

    devicePort.Write(master ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHighPort.Write(0);
    cmdPort.Write(0xEC);

    status = cmdPort.Read();
    if(status == 0x00) return; // no device

    while(((status& 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = cmdPort.Read(); //wait until data is ready

    if(status & 0x01){
        printf("ERROR");
        return;
    }

    // if we reach here that means data is ready
    for(uint16_t i = 0; i< 256; i++){
        uint16_t data = dataPort.Read();
        char* foo = " \0";
        foo[1] = (data >> 8) & 0x00FF;
        foo[0] = data & 0x00FF;
        printf(foo);
    }
}
void AdvancedTechnologyAttachment::Read28(common::uint32_t sector,common::uint8_t* data, int count){
    if(sector & 0xF0000000) return; //checking first 4 bits of sector if address is larger than 28 bits

    if(count> bytesPerSector) return; //not allow to write too much data
    devicePort.Write((master ? 0xE0 : 0xF0) | ((sector & 0X0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(0);


    lbaLowPort.Write((sector & 0x000000FF));
    lbaMidPort.Write((sector & 0x0000FF00) >> 8);
    lbaHighPort.Write((sector & 0x00FF0000) >>16);
    cmdPort.Write(0x20);

    uint8_t status = cmdPort.Read();
    if(status == 0x00) return; // no device

    while(((status& 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = cmdPort.Read(); //wait until data is ready

    if(status & 0x01){
        printf("ERROR");
        return;
    }

    printf("Reading from ATA: ");

    for(uint16_t i = 0; i< count; i+= 2){
        uint16_t wdata = dataPort.Read();

        char* foo = " \0";
        foo[1] = (wdata >> 8) & 0x00FF;
        foo[0] = wdata & 0x00FF;
        printf(foo);

        data[i] = wdata & 0x00FF;
        if(i+1 < count) data[i+1] = (wdata >> 8)&0x00FF;
    }
    for(uint16_t i = count + (count%2); i< bytesPerSector; i+= 2){
        dataPort.Read();
    }
}
void AdvancedTechnologyAttachment::Write28(common::uint32_t sector,common::uint8_t* data, int count){
    
    if(sector & 0xF0000000) return; //checking first 4 bits of sector if address is larger than 28 bits

    if(count> bytesPerSector) return; //not allow to write too much data
    devicePort.Write((master ? 0xE0 : 0xF0) | ((sector & 0X0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(0);


    lbaLowPort.Write((sector & 0x000000FF));
    lbaMidPort.Write((sector & 0x0000FF00) >> 8);
    lbaHighPort.Write((sector & 0x00FF0000) >>16);
    cmdPort.Write(0x30);

    printf("Writing to ATA: ");

    for(uint16_t i = 0; i< count; i+= 2){
        uint16_t wdata = data[i];
        if(i+1 < count) wdata |= ((uint16_t)data[i+1]) << 8;

        char* foo = " \0";
        foo[1] = (wdata >> 8) & 0x00FF;
        foo[0] = wdata & 0x00FF;
        printf(foo);

        dataPort.Write(wdata);
    }
    for(uint16_t i = count + (count%2); i< bytesPerSector; i+= 2){
        dataPort.Write(0x0000);
    }
}
void AdvancedTechnologyAttachment::Flush(){
    devicePort.Write(master ? 0xE0 : 0xF0);
    cmdPort.Write(0xE7);

    uint8_t status = cmdPort.Read();
    while(((status& 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = cmdPort.Read(); //wait until device is flushed

    if(status & 0x01){
        printf("ERROR");
        return;
    }
}
