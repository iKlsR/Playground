#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include <QWidget>
#include <QPaintEvent>

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    ColorPicker(QWidget *parent = Q_NULLPTR);
    ~ColorPicker();

    void paintEvent(QPaintEvent*) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    QPainter *_painter;
};

#endif
