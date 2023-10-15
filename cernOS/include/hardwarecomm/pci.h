#ifndef __CERNOS__HARDWARECOMM__PCI_H
#define __CERNOS__HARDWARECOMM__PCI_H
    #include <common/types.h>
    #include <hardwarecomm/port.h>
    #include <hardwarecomm/interrupts.h>
    #include <drivers/driver.h>

namespace cernos{
    namespace hardwarecomm{

        enum BaseAddressRegisterType{
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister{
            public:
                bool prefatchable;
                cernos::common::uint8_t* address;
                cernos::common::uint32_t size;
                BaseAddressRegisterType type;
        };

        class PCIDeviceDescriptor{
            public:
                cernos::common::uint32_t portBase;
                cernos::common::uint32_t interrupt;

                cernos::common::uint16_t bus;
                cernos::common::uint16_t device;
                cernos::common::uint16_t func;

                cernos::common::uint16_t vendor_id;
                cernos::common::uint16_t device_id;

                cernos::common::uint8_t class_id;
                cernos::common::uint8_t subclass_id;
                cernos::common::uint8_t interface_id;

                cernos::common::uint16_t revision;

                PCIDeviceDescriptor();
                ~PCIDeviceDescriptor();

        };

        class PCIController{
            Port32bit dataPort;
            Port32bit cmdPort;
            public:
                PCIController();
                ~PCIController();

                cernos::common::uint32_t Read(cernos::common::uint16_t bus, cernos::common::uint16_t device, cernos::common::uint16_t func, cernos::common::uint32_t offset);
                void Write(cernos::common::uint16_t bus, cernos::common::uint16_t device, cernos::common::uint16_t func, cernos::common::uint32_t offset, cernos::common::uint32_t data);
                bool DeviceCheck(cernos::common::uint16_t bus, cernos::common::uint16_t device);
                
                void SelectDrivers(cernos::drivers::DriverManager* dm, cernos::hardwarecomm::InterruptManager* im);
                cernos::drivers::Driver* GetDriver(PCIDeviceDescriptor dev,cernos::hardwarecomm::InterruptManager* im);
                PCIDeviceDescriptor GetDeviceDescriptor(cernos::common::uint16_t bus, cernos::common::uint16_t device, cernos::common::uint16_t func);

                BaseAddressRegister GetBaseAddressRegister(cernos::common::uint16_t bus, cernos::common::uint16_t device, cernos::common::uint16_t func, cernos::common::uint16_t bar);
        };
    }
}
    
#endif 