#ifndef __CERNOS__GUI__DESKTOP_H
#define __CERNOS__GUI__DESKTOP_H
#include <gui/widget.h>
#include <drivers/mouse.h>
namespace cernos{
    namespace gui{
        class Desktop : public CompositeWidget, public cernos::drivers::MouseEventHandler{
            protected:
                common::uint32_t MouseX;
                common::uint32_t MouseY;
            public:
                Desktop(common::uint32_t w, common::uint32_t h,common::uint8_t r,common::uint8_t g,common::uint8_t b);
                ~Desktop();

                void Draw(common::GraphicsContext* gc);

                void OnMouseDown(cernos::common::uint8_t button);
				void OnMouseUp(cernos::common::uint8_t button);
				void OnMouseMove(int x, int y);
        };
    }
}

#endif