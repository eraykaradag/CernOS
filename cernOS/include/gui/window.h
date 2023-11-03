#ifndef __CERNOS__GUI__WINDOW_H
#define __CERNOS__GUI__WINDOW_H
#include <gui/widget.h>
#include <drivers/mouse.h>
namespace cernos{
    namespace gui{
        class Window : public CompositeWidget{
            protected:
                bool dragging;
            public:
            Window(Widget* parent,common::uint32_t x,common::uint32_t y,common::uint32_t w,common::uint32_t h,common::uint8_t r,common::uint8_t g,common::uint8_t b);
            ~Window();

            void OnMouseDown(common::uint32_t x,common::uint32_t y,common::uint8_t button);
            void OnMouseUp(common::uint32_t x,common::uint32_t y,common::uint8_t button);
            void OnMouseMove(common::uint32_t oldx,common::uint32_t oldy,common::uint32_t newx,common::uint32_t newy);

        };
    }
}
#endif