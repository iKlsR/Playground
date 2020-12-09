#include "Button.hpp"

Button::Button(const QString &button_text,
               ButtonType button_type,
               ButtonStyle button_style,
               QWidget *parent)
    : QPushButton(button_text, parent)
{
    _button_text = button_text;
    _button_type = button_type;
    _button_style = button_style;

    switch (button_type) {
        case ButtonType::PL_SUCCESS:
            setAccessibleName("success");
        break;
        case ButtonType::PL_ERROR:
            setAccessibleName("error");
        break;
        case ButtonType::PL_DEFAULT:
        default:
            setAccessibleName("default");
        break;
    };

    setCursor(Qt::PointingHandCursor);
}

Button::~Button()
{

}
