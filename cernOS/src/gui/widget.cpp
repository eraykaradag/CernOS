#include <gui/widget.h>

using namespace cernos::common;
using namespace cernos::gui;

Widget::Widget(Widget* parent,uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b)
: KeyboardEventHandler()
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->Focussable = true;
}
Widget::~Widget(){

}
void Widget::GetFocus(Widget* widget){
    if(parent != 0){
        parent->GetFocus(widget);
    }
}
void Widget::ModelToScreen(uint32_t &x,uint32_t &y){
    if(parent != 0){
        parent->ModelToScreen(x,y);
    }
    x += this->x;
    y += this->y;
}            
void Widget::Draw(GraphicsContext* gc){
    uint32_t X = 0;
    uint32_t Y = 0;
    ModelToScreen(X,Y);
    gc->FillRectangle(X,Y,w,h,r,g,b);
}
void Widget::OnMouseDown(uint32_t x,uint32_t y, uint8_t button){
    if(Focussable)
        GetFocus(this);
}

bool Widget::ContainsCoordinate(uint32_t x,uint32_t y){
    return this->x <= x && x < this->x + this->w && this->y <= y && y < this->y + this->h;
}
void Widget::OnMouseUp(uint32_t x,uint32_t y,uint8_t button){

}
void Widget::OnMouseMove(uint32_t oldx,uint32_t oldy,uint32_t newx,uint32_t newy){

}


CompositeWidget::CompositeWidget(Widget* parent,uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b)
: Widget(parent,x,y,w,h,r,g,b)
{
    focussedChild = 0;
    numChildren = 0;
}
CompositeWidget::~CompositeWidget(){

}
bool CompositeWidget::AddChild(Widget* child){
    if(numChildren >= 100){
        return false;
    }
    Children[numChildren++] = child;
    return true;
}

void CompositeWidget::GetFocus(Widget* widget){
    this->focussedChild = widget;
    if(parent != 0)
        parent->GetFocus(this);
}
void CompositeWidget::Draw(GraphicsContext* gc){
    Widget::Draw(gc);
    for(int i = numChildren-1; i>= 0; i--){
        Children[i]->Draw(gc);
    }
}
void CompositeWidget::OnMouseDown(uint32_t x,uint32_t y,uint8_t button){
    for(int i = 0; i <numChildren; i++){
        if(Children[i]->ContainsCoordinate(x-this->x,y - this->y)){
            Children[i]->OnMouseDown(x-this->x,y - this->y,button);
            break;
        }
    }
}
void CompositeWidget::OnMouseUp(uint32_t x,uint32_t y,uint8_t button){
    for(int i = 0; i <numChildren; i++){
        if(Children[i]->ContainsCoordinate(x-this->x,y - this->y)){
            Children[i]->OnMouseUp(x-this->x,y - this->y,button);
            break;
        }
    }
}
void CompositeWidget::OnMouseMove(uint32_t oldx,uint32_t oldy,uint32_t newx,uint32_t newy){
    int firstchild = -1;
    for(int i = 0; i <numChildren; i++){
        if(Children[i]->ContainsCoordinate(oldx-this->x,oldy - this->y)){
            Children[i]->OnMouseMove(oldx-this->x,oldy - this->y, newx-this->x,newy - this->y);
            firstchild = i;
            break;
        }
    }
    for(int i = 0; i <numChildren; i++){
        if(Children[i]->ContainsCoordinate(newx-this->x,newy - this->y)){
            if(firstchild != i)
                Children[i]->OnMouseMove(oldx-this->x,oldy - this->y, newx-this->x,newy - this->y);
            break;
        }
    }
}
void CompositeWidget::OnKeyDown(char str){
    if(focussedChild != 0)
        focussedChild->OnKeyDown(str);
}
void CompositeWidget::OnKeyUp(char str){
    if(focussedChild != 0)
        focussedChild->OnKeyUp(str);
}