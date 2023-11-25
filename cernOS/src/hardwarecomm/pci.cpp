#include <hardwarecomm/pci.h>
#include <drivers/amd_am79c973.h>

using namespace cernos::common;
using namespace cernos::drivers;
using namespace cernos::hardwarecomm;

void printf(char*);
void printfHex(uint8_t);

PCIDeviceDescriptor::PCIDeviceDescriptor(){

}
PCIDeviceDescriptor::~PCIDeviceDescriptor(){

}


PCIController::PCIController()
: dataPort(0xCFC),cmdPort(0xCF8)
{

}

PCIController::~PCIController(){

}

uint32_t PCIController::Read(uint16_t bus, uint16_t device, uint16_t func, uint32_t offset){
    uint32_t id = 
        0x1<<31
        |((bus & 0xFF) <<16)
        |((device & 0x1F) << 11)
        |((func & 0x07)  << 8)
        |(offset & 0xFC);
    cmdPort.Write(id);
    uint32_t result = dataPort.Read();

    return result >> (8*(offset % 4)); 
}

void PCIController::Write(uint16_t bus, uint16_t device, uint16_t func, uint32_t offset, uint32_t data){
    uint32_t id = 
        0x1<<31
        |((bus & 0xFF) <<16)
        |((device & 0x1F) << 11)
        |((func & 0x07)  << 8)
        |(offset & 0xFC);
    cmdPort.Write(id);
    dataPort.Write(data);
}

bool PCIController::DeviceCheck(uint16_t bus, uint16_t device){
    return Read(bus,device,0,0x0E) & (1<<7);
}
void PCIController::SelectDrivers(DriverManager* dm, InterruptManager* im){

    for(int bus = 0; bus<8; bus++){
        for(int device = 0; device < 32; device++){
            int numFuncs = DeviceCheck(bus, device) ? 8 : 1;
            for(int func = 0; func < numFuncs; func++){
                PCIDeviceDescriptor dev = GetDeviceDescriptor(bus,device,func);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;
                
                for(int barNum = 0; barNum<6; barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus,device,func,barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;
                }
                Driver* driver = GetDriver(dev,im);
                if(driver != 0)
                    dm->AddDriver(driver);


                /*printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(func & 0xFF);

                printf(", VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);

                printf("\n");*/
                

            }
        }
    }
}

Driver* PCIController::GetDriver(PCIDeviceDescriptor dev,InterruptManager* im){
    Driver* driver = 0;
    switch(dev.vendor_id){
        case 0x1022:// AMD
            switch(dev.device_id){
                case 0x2000: //am79c973
                    driver = (Driver*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79c973));
                    printf("memory allocated \n");
                    if(driver != 0)
                        new (driver) amd_am79c973(&dev,im);
                    return driver;
                    break;
            }
            break;
        case 0x8086: //intel
            //printf("INTEL \n");
            break;
    }

    switch(dev.class_id){
        case 0x03: //graphics
            switch(dev.subclass_id){
                case 0x00: //VGA
                    //printf("VGA\n");
                    break;
            }
            break;
    }

    return driver;
}

BaseAddressRegister PCIController::GetBaseAddressRegister(cernos::common::uint16_t bus, cernos::common::uint16_t device, cernos::common::uint16_t func, cernos::common::uint16_t bar){
   BaseAddressRegister result;

    uint32_t headerType = Read(bus,device,func,0x0E) &0x7F;
    int maxBARs = 6-(4*headerType);
    if(bar >= maxBARs){
        return result;
    }
    uint32_t bar_value = Read(bus, device,func,0x10+4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if(result.type == MemoryMapping){
        switch((bar_value>>1) & 0x3){
            case 0://32 bit mode
            case 1://20 bit mode
            case 2://64 bit mode
            break;
        }
        result.prefatchable = ((bar_value>>3) & 0x1) == 0x1;
    }
    else{// I/O
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefatchable = false;
    }
   return result; 
}


PCIDeviceDescriptor PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t func){
    PCIDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.func = func;

    result.vendor_id = Read(bus,device,func,0x00);
    result.device_id = Read(bus,device,func,0x02);

    result.class_id = Read(bus,device,func,0x0B);
    result.subclass_id = Read(bus,device,func,0x0A);
    result.interface_id = Read(bus,device,func,0x09);

    result.revision = Read(bus,device,func,0x08);
    result.interrupt = Read(bus,device,func,0x3C);

    return result;
}
