#ifndef PL_BUTTON_HPP
#define PL_BUTTON_HPP

#include <QPushButton>

enum class ButtonStyle
{
    PL_ICON,
    PL_TEXT,
};

enum class ButtonType {
    PL_DEFAULT = 0,
    PL_SUCCESS,
    PL_ERROR,
};

class Button : public QPushButton
{
    Q_OBJECT

public:
    explicit Button(const QString &button_text,
                    ButtonType button_type = ButtonType::PL_DEFAULT,
                    ButtonStyle button_style = ButtonStyle::PL_TEXT,
                    QWidget *parent = Q_NULLPTR);
    ~Button();

private:
    ButtonType  _button_type;
    ButtonStyle _button_style;
    QString     _button_text;
};

#endif // PL_BUTTON_HPP
