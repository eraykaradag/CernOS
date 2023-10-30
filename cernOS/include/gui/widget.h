#ifndef __CERNOS__GUI__WIDGET_H
#define __CERNOS__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>

namespace cernos{
    namespace gui{

        class Widget{
            protected:
                Widget* parent;
                common::uint32_t x;
                common::uint32_t y;
                common::uint32_t w;
                common::uint32_t h;

                common::uint8_t r;
                common::uint8_t g;
                common::uint8_t b;
                bool Focussable;
            public:
                Widget(Widget* parent,common::uint32_t x,common::uint32_t y,common::uint32_t w,common::uint32_t h,common::uint8_t r,common::uint8_t g,common::uint8_t b);
                ~Widget();
                virtual void GetFocus(Widget* widget);
                virtual void ModelToScreen(common::uint32_t &x,common::uint32_t &y);
            
                virtual void Draw(common::GraphicsContext* gc);
                virtual void OnMouseDown(common::uint32_t x,common::uint32_t y);
                virtual void OnMouseUp(common::uint32_t x,common::uint32_t y);
                virtual void OnMouseMove(common::uint32_t oldx,common::uint32_t oldy,common::uint32_t newx,common::uint32_t newy);
                virtual bool ContainsCoordinate(common::uint32_t x,common::uint32_t y);

                virtual void OnKeyDown(char* str);
                virtual void OnKeyUp(char* str);
        };

        class CompositeWidget : public Widget{
            private:
                Widget* Children[100];
                int numChildren;
                Widget* focussedChild;
            public:
                CompositeWidget(Widget* parent,common::uint32_t x,common::uint32_t y,common::uint32_t w,common::uint32_t h,common::uint8_t r,common::uint8_t g,common::uint8_t b);
                ~CompositeWidget();
                virtual void GetFocus(Widget* widget);
            
                virtual void Draw(common::GraphicsContext* gc);
                virtual void OnMouseDown(common::uint32_t x,common::uint32_t y);
                virtual void OnMouseUp(common::uint32_t x,common::uint32_t y);
                virtual void OnMouseMove(common::uint32_t oldx,common::uint32_t oldy,common::uint32_t newx,common::uint32_t newy);

                virtual void OnKeyDown(char* str);
                virtual void OnKeyUp(char* str);
        };
    }
}


#endif