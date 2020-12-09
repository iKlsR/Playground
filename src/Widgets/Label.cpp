#include "Label.hpp"

Label::Label(const QString &text, QWidget *parent) : _text(text), QLabel(text, parent)
{
    setAccessibleName("label_widget");
}

Label::~Label()
{

}