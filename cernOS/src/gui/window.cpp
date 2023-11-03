#include <gui/window.h>

using namespace cernos::common;
using namespace cernos::gui;

Window::Window(Widget* parent,uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b)
: CompositeWidget(parent, x,y,w,h,r,g,b)
{
    dragging = false;
}
Window::~Window(){

}
void Window::OnMouseDown(uint32_t x,uint32_t y,uint8_t button){
    dragging = button == 1;
    CompositeWidget::OnMouseDown(x,y,button);

}
void Window::OnMouseUp(uint32_t x,uint32_t y,uint8_t button){
    dragging = false;
    CompositeWidget::OnMouseUp(x,y,button);
}
void Window::OnMouseMove(uint32_t oldx,uint32_t oldy,uint32_t newx,uint32_t newy){
    if(dragging){
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
    CompositeWidget::OnMouseMove(oldx,oldy,newx,newy);
}