#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QLinearGradient>

#include "ColorPicker.hpp"

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    // adjustSize();
}

ColorPicker::~ColorPicker()
{
    // what the fuck??
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QRect rect = event->rect();

    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);
    // painter.setPen(Qt::black);
    // painter.drawText(rect, Qt::AlignCenter,
    //                   "Data");
    // painter.drawRect(rect);
    QLinearGradient linearGrad(QPointF(rect.x(), rect.y()),
                               QPointF(rect.width(), rect.height()));
    linearGrad.setColorAt(0, Qt::white);
    linearGrad.setColorAt(1, Qt::green);

    painter.fillRect(rect, linearGrad);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

QSize ColorPicker::sizeHint() const
{
    return QSize(256, 128);
}

QSize ColorPicker::minimumSizeHint() const
{
    return QSize(128, 128);
}
