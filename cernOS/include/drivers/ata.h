#ifndef __CERNOS__DRIVERS__ATA_H
#define __CERNOS__DRIVERS__ATA_H

#include <hardwarecomm/port.h>
#include <common/types.h>
    namespace cernos{
        namespace drivers{
            class AdvancedTechnologyAttachment{
                protected:
                    hardwarecomm::Port16bit dataPort;
                    hardwarecomm::Port8bit errorPort;
                    hardwarecomm::Port8bit sectorCountPort;
                    hardwarecomm::Port8bit lbaLowPort;
                    hardwarecomm::Port8bit lbaMidPort;
                    hardwarecomm::Port8bit lbaHighPort;
                    hardwarecomm::Port8bit devicePort;
                    hardwarecomm::Port8bit cmdPort;
                    hardwarecomm::Port8bit controlPort;

                    bool master;
                    common::uint16_t bytesPerSector;
                public:
                    AdvancedTechnologyAttachment(common::uint16_t portBase, bool master);
                    ~AdvancedTechnologyAttachment();
                    void Identify();
                    void Read28(common::uint32_t sector,common::uint8_t* data, int count);
                    void Write28(common::uint32_t sector,common::uint8_t* data, int count);
                    void Flush();
            };
        }
    }
#endif