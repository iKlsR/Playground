#ifndef PL_SLIDER_HPP
#define PL_SLIDER_HPP

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT;

public:
    Slider(Qt::Orientation o = Qt::Horizontal, QWidget *parent = Q_NULLPTR);
    ~Slider();

private:
    QString _text;
};

#endif // PL_SLIDER_HPP