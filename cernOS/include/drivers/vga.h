#ifndef __CERNOS__DRIVERS__VGA_H
#define __CERNOS__DRIVERS__VGA_H
#include <common/types.h>
#include <hardwarecomm/port.h>
#include <drivers/driver.h>
namespace cernos{
	namespace drivers{
        class VideoGraphicsArray{
            protected:
                cernos::hardwarecomm::Port8bit miscPort;
                cernos::hardwarecomm::Port8bit crtcIndexPort;
                cernos::hardwarecomm::Port8bit crtcDataPort;
                cernos::hardwarecomm::Port8bit sequencerIndexPort;
                cernos::hardwarecomm::Port8bit sequencerDataPort;
                cernos::hardwarecomm::Port8bit graphicsControllerIndexPort;
                cernos::hardwarecomm::Port8bit graphicsControllerDataPort;
                cernos::hardwarecomm::Port8bit attributeControllerIndexPort;
                cernos::hardwarecomm::Port8bit attributeControllerReadPort;
                cernos::hardwarecomm::Port8bit attributeControllerWritePort;
                cernos::hardwarecomm::Port8bit attributeControllerResetPort;

                void WriteRegisters(cernos::common::uint8_t* registers);
                cernos::common::uint8_t* GetFrameBufferSegment();
                
                virtual cernos::common::uint8_t GetColorIndex(cernos::common::uint8_t r, cernos::common::uint8_t g, cernos::common::uint8_t b);
            public:
                VideoGraphicsArray();
                ~VideoGraphicsArray();

                virtual bool SupportsMode(cernos::common::uint32_t width,cernos::common::uint32_t height, cernos::common::uint32_t colordepth);
                virtual bool SetMode(cernos::common::uint32_t width,cernos::common::uint32_t height, cernos::common::uint32_t colordepth);
                virtual void PutPixel(cernos::common::uint32_t x, cernos::common::uint32_t y, cernos::common::uint8_t colorIndex);
                virtual void PutPixel(cernos::common::uint32_t x, cernos::common::uint32_t y,cernos::common::uint8_t r, cernos::common::uint8_t g, cernos::common::uint8_t b);

                virtual void FillRectangle(cernos::common::uint32_t x, cernos::common::uint32_t y,cernos::common::uint32_t w,cernos::common::uint32_t h,cernos::common::uint8_t r, cernos::common::uint8_t g, cernos::common::uint8_t b);

        };
    }
}
#endif