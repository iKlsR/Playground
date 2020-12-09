#include "Slider.hpp"

Slider::Slider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{
    setRange(0, 10);
    setValue(5);
}

Slider::~Slider()
{

}